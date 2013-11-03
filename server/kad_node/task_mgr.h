#pragma once

namespace oo
{
    struct SingleThreadMutex
    {
        void lock(){}
        bool trylock(){return true;}
        void unlock() {}

        typedef boost::unique_lock<SingleThreadMutex> scoped_lock;
    };

    typedef boost::function< void ()>               MsgPortTask;
    typedef boost::function< void (MsgPortTask) >   ResumeMsgPortTask;
    typedef boost::function< void (int)>            TimerTask;
    class MsgPortTaskManager
    {
    public:
        struct TaskProperties
        {
            MsgPortTask task;
            ulong worker;
            int priority;
            bool btimer;
        };

        struct WaitCtrl
        {
            MsgPortTask task;
            condition *weaker;
            uuid id;
            ulong timeout;
        };

        struct TimerProperties
        {
            MsgPortTask task;
            ulong group;
            ulong id;
            int priority;
            int gap;
            int life;
            boost::system_time::time_duration_type::tick_type last;
        };

    private:
        // 任务队列
        std::list<TaskProperties>   mTaskLst;
        // 工作线程池
        std::list<thread*>      mThreadLst;
        // 定时器
        std::list<TimerProperties>    mTimerList;
        std::list<TimerProperties>    mTimerWaitList;
        std::set<ulong>               mWorkerCtrl;             

        thread*               mTimer;
        mutex                 mQueueMutex;
        mutex                 mTimerMutex;
        condition             mWeaker;
        int                   mnProcess;
        bool                  mbStop;
        bool                  mbPause;
        bool                  mbPauseTimer;
    public:
        static MsgPortTaskManager& instance();

        void startup(int nthread);
        void pause(bool bpause);
        void pauseTimer(bool bpause);
        void stop();

        void addTask(MsgPortTask task, ulong worker = -1, int priority = -1);
        int addTimer(TimerTask task, int cycletime, int life = -1, ulong worker = -1, int priority = -1);

        void clearAllTask();
        void clearAllTimer();

        void killTimer(int id);
        void update();

    protected:
        MsgPortTask getTask(ulong& lastworker);
        void _addTask(MsgPortTask MsgPortTask, ulong worker, int priority, bool btimer = false);
        bool _getTask(MsgPortTask& rt, ulong& group);
        void taskproc(int i);
    };

    inline void tm_pause(bool bpause){
        MsgPortTaskManager::instance().pause(bpause);
    }

    inline void tm_pause_timer(bool bpause){
        MsgPortTaskManager::instance().pauseTimer(bpause);
    }

    inline void tm_add_task(MsgPortTask task, ulong worker = -1, int priority = -1){
        MsgPortTaskManager::instance().addTask(task, worker, priority);
    }
    inline int tm_add_timer(TimerTask task, int cycletime, int life = -1, ulong worker = -1, int priority = -1){
        MsgPortTaskManager::instance().addTimer(task, cycletime, life, worker, priority);
    }

}

