#include "stdafx.h"
#include "task_mgr.h"
#include "MsgDispatchor.h"


namespace oo{
    void MsgDispatchor::lockDispatch()
    {
        mDispatchMutex.lock();
    }

    void MsgDispatchor::unlockDispatch()
    {
        mDispatchMutex.unlock();
    }

    bool MsgDispatchor::tryLockDispatch()
    {
        return mDispatchMutex.try_lock();
    }

    void MsgDispatchor::setTaskDispatchor(const std::string& id, MsgProc dis, int life, ulong worker, int priority)
    {
        mutex::scoped_lock sl(mDispatchMutex);
        Dispatchor dispa;
        dispa.func = dis;
        dispa.worker = worker;
        dispa.priority = priority;
        dispa.life = life;
        mDispatchors.insert(MsgProcTbl::value_type(id,dispa) );
    }

    void MsgDispatchor::remDispatchor(const std::string& id)
    {
        mutex::scoped_lock sl(mDispatchMutex);
        mDispatchors.erase(id);
    }

    bool MsgDispatchor::dispatch(const std::string& from, const std::string& to, Message* msg)
    {
        mDispatchMutex.lock();
        MsgProcTbl::iterator cit = mDispatchors.find(to);
        if( cit != mDispatchors.end() )
        {
            Dispatchor dispa = cit->second;
            if( cit->second.life != -1 )
            {
                cit->second.life--;
                if( cit->second.life <= 0 )
                    mDispatchors.erase(cit);
            }
            mDispatchMutex.unlock();

            _dispatch(from, to, msg, dispa);
            return true;
        }
        else
        {
            mDispatchMutex.unlock();
            return false;
        }
    }

    int MsgDispatchor::_dispatch(const std::string& from, const std::string& to, Message* msg, Dispatchor& da)
    {
        // simple
//        da.func(from, to, msg);
        MsgPortTaskManager::instance().addTask(boost::bind(da.func, from, to, msg), da.worker, da.priority);
        return 0;
    }

    void MsgDispatchor::wait(const std::string& id, std::string& ppWait)
    {
        //setDirectDispatchor(id, boost::bind(&_waitDispatchor, ppWait, _1), 1);
    }
}
