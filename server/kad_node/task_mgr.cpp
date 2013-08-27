#include "stdafx.h"
#include "task_mgr.h"

namespace oo{

    MsgPortTaskManager& MsgPortTaskManager::instance()
    {
        static MsgPortTaskManager __Inst__;
        return __Inst__;
    }

    void MsgPortTaskManager::startup(int nthread)
    {
        mnProcess = nthread;
        mbStop = false;
        mbPause = false;
        mbPauseTimer = false;
        for(int i = 0; i < nthread; i++)
        {
            thread* th = new thread(boost::bind(&MsgPortTaskManager::taskproc, this, i+1));
            mThreadLst.push_back(th);
        }

        mTimer = new thread(boost::bind(&MsgPortTaskManager::update, this) );
    }

    void MsgPortTaskManager::clearAllTimer()
    {
        mutex::scoped_lock sl(mTimerMutex);
        mTimerList.clear();
    }

    void MsgPortTaskManager::pauseTimer(bool bpause)
    {
        mutex::scoped_lock sl(mTimerMutex);
        if(mbPauseTimer == bpause)
            return;
        mbPauseTimer = bpause;
    }

    void MsgPortTaskManager::update()
    {
        while(!mbStop)
        {
            mTimerMutex.lock();
            if(!mbPauseTimer)
            {
                boost::system_time st = boost::get_system_time();
                boost::system_time::time_duration_type::tick_type now = st.time_of_day().total_milliseconds();
                if(mTimerWaitList.empty())
                {// wait
                    for(std::list<TimerProperties>::iterator iter = mTimerList.begin(); iter != mTimerList.end(); iter++)
                    {
                        if(now - iter->last > iter->gap)
                        {
                            iter->last = now;
                            mTimerWaitList.insert(mTimerWaitList.end(), *iter);
                        }
                    }
                }

                mTimerMutex.unlock();
                // dispatch timer
                if(!mTimerWaitList.empty())
                {
                    mQueueMutex.lock();
                    for(std::list<TimerProperties>::iterator iter = mTimerWaitList.begin(); iter != mTimerWaitList.end(); iter++)
                    {
                        _addTask(iter->task, iter->group, iter->priority, true);
                    }
                    mTimerWaitList.clear();
                    mWeaker.notify_all();   
                    mQueueMutex.unlock();
                }
            }
            else
            {
                mTimerMutex.unlock();
            }

            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }
    }

    int MsgPortTaskManager::addTimer(TimerTask task, int cycletime, ulong group, int priority)
    {
        static int __curId = 0;
        mutex::scoped_lock sl(mTimerMutex);
        TimerProperties tp;
        tp.id = __curId++;
        tp.task = boost::bind(task, tp.id);
        tp.gap = cycletime;
        tp.group = group;
        tp.priority = priority;
        
        mTimerList.push_back(tp);
        return tp.id;
    }

    void MsgPortTaskManager::killTimer(int id)
    {
        mutex::scoped_lock sl(mTimerMutex);
        for(std::list<TimerProperties>::iterator iter = mTimerList.begin(); iter != mTimerList.end(); iter++)
        {
            if(iter->id == id)
                mTimerList.erase(iter);
        }
    }

    void MsgPortTaskManager::stop()
    {
        mbStop = true;
        pause(true);
        pauseTimer(true);

        // 定时器停止
        if(mTimer)
        {
            mTimer->join();
            delete mTimer;
            mTimer = NULL;
        }

        mTaskLst.clear();
        mTimerWaitList.clear();
        mWorkerCtrl.clear();

        mWeaker.notify_all();
        for(std::list<thread*>::iterator i = mThreadLst.begin(); i != mThreadLst.end(); i++)
        {
            (*i)->join();
            delete (*i);
        }
        mThreadLst.clear();
    }   

    struct _TaskPriority
    {
        bool operator()(int pri, const MsgPortTaskManager::TaskProperties& tp)
        {
            return pri < tp.priority;
        }

        bool operator()(const MsgPortTaskManager::TaskProperties& tp, int pri)
        {
            return tp.priority < pri;
        }

        bool operator()(const MsgPortTaskManager::TaskProperties& left, const MsgPortTaskManager::TaskProperties& right)
        {
            return left.priority < right.priority;
        }
    };

    void MsgPortTaskManager::_addTask(MsgPortTask task, ulong worker, int priority, bool btimer)
    {
        TaskProperties tp;
        tp.task = task;
        tp.priority = priority;
        tp.worker = worker;
        tp.btimer = btimer;

        if(mTaskLst.empty() || priority != -1)  // 需要排序
        {
            std::list<TaskProperties>::iterator iti = std::upper_bound(mTaskLst.begin(), mTaskLst.end(), priority, _TaskPriority());
            mTaskLst.insert(iti, tp);
        }
        else
        {
            mTaskLst.push_back(tp);
        }
    }

    void MsgPortTaskManager::clearAllTask()
    {
        mutex::scoped_lock sl(mQueueMutex);
        mTaskLst.clear();
    }

    bool MsgPortTaskManager::_getTask(MsgPortTask& rt, ulong& worker)
    {
        if(mTaskLst.empty() || mbPause)
            return false;

        for(std::list<TaskProperties>::iterator it = mTaskLst.begin(); it != mTaskLst.end(); it++)
        {
            if(it->worker == -1 || mWorkerCtrl.find(it->worker) == mWorkerCtrl.end())
            {
                rt = it->task;
                worker = it->worker;
                mWorkerCtrl.insert(worker);
                mTaskLst.erase(it);
                return true;
            }
        }
        return false;
    }

    void MsgPortTaskManager::addTask(MsgPortTask task, ulong worker, int priority)
    {
        mutex::scoped_lock sl(mQueueMutex);

        bool ball = !mTaskLst.empty();

        _addTask(task, worker, priority);

        if(!mbPause)    //
        {
            if(ball)
                mWeaker.notify_all();   
            else
                mWeaker.notify_one();
        }
    }

    MsgPortTask MsgPortTaskManager::getTask(ulong& lastworker)
    {
        mutex::scoped_lock sl(mQueueMutex);
        if(lastworker != -1)                 // 完成任务，注销某个group标记，使得该group的认为可以被分发
            mWorkerCtrl.erase(lastworker);

        MsgPortTask rt; 
        if(!_getTask(rt, lastworker))
            mWeaker.wait(sl);
        else
        {
            return rt;
        }

        // wait for task
        _getTask(rt, lastworker);
        return rt;
    }

    void MsgPortTaskManager::taskproc(int i)
    {
        // just suspend
        ulong lastworker = -1;
        while(!mbStop)
        {
            mnProcess--;
            MsgPortTask pt = getTask(lastworker);
            mnProcess++;
            if(pt)
                pt();
        }
        mnProcess--;
    }

    void MsgPortTaskManager::pause(bool bpause)
    {
        mutex::scoped_lock sl(mQueueMutex);
        if(mbPause == bpause)
            return;
        mbPause = bpause;
        if(!mbPause)
        {
            if(!mTaskLst.empty())
                mWeaker.notify_all();   
        }
        else    // wait all thread enter waiting state, then return
        {
            while(1)
            {
                if(mnProcess == 0)
                    break;
                boost::this_thread::sleep(boost::posix_time::milliseconds(1));
            }
        }
    }
}
