#include "stdafx.h"
#include "MsgDispatchor.h"
#include "task_mgr.h"

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

    void MsgDispatchor::setDirectDispatchor(msgid id, MsgProc dis, int life, ulong worker, int priority)
    {
        mutex::scoped_lock sl(mDispatchMutex);
        Dispatchor dispa;
        dispa.func = dis;
        dispa.worker = worker;
        dispa.priority = priority;
        dispa.life = life;
        mDispatchors.insert(MsgProcTbl::value_type(id,dispa) );
    }

    void MsgDispatchor::remDispatchor(msgid id)
    {
        mutex::scoped_lock sl(mDispatchMutex);
        mDispatchors.erase(id);
    }

    bool MsgDispatchor::dispatch(msgid id, const std::string& from, const std::string& to, const std::string& buf)
    {
        mDispatchMutex.lock();
        MsgProcTbl::iterator cit = mDispatchors.find(id);
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

            _dispatch(id, from, to, buf, dispa);
            return true;
        }
        else
        {
            mDispatchMutex.unlock();
            return false;
        }
    }

    int MsgDispatchor::_dispatch(msgid id, const std::string& from, const std::string& to, const std::string& buf, Dispatchor& da)
    {
        // simple
        da.func(from, to, buf);
        return 0;
    }

    void _waitDispatchor(std::string& ppWait, const std::string& msg)
    {
        ppWait = msg;
    }

    void _managedDispatchor(msgid id, MsgProc proc, int worker, int priority, Message* msg)
    {
        MsgPortTaskManager::instance().addTask(boost::bind(proc, msg), worker, priority);
    }

    void MsgDispatchor::wait(msgid id, std::string& ppWait)
    {
        //setDirectDispatchor(id, boost::bind(&_waitDispatchor, ppWait, _1), 1);
    }

    void MsgDispatchor::setTaskDispatchor(msgid id, MsgProc proc, int life, ulong worker, int priority)
    {
        setDirectDispatchor(id, boost::bind(&_managedDispatchor, id, proc, worker, priority, _1), life, worker, priority);
    }

}
