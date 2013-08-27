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

    void MsgDispatchor::dispatch(msgid id, Message* msg)
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

            _dispatch(id, msg, dispa);
        }
        else
        {
            mDispatchMutex.unlock();
        }
    }

    void MsgDispatchor::_dispatch(msgid id, Message* msg, Dispatchor& da)
    {
        // simple
        da.func(msg);
    }

    void _waitDispatchor(Message** ppWait, Message* msg)
    {
        *ppWait = msg;
    }

    void _managedDispatchor(msgid id, MsgProc proc, int worker, int priority, Message* msg)
    {
        MsgPortTaskManager::instance().addTask(boost::bind(proc, msg), worker, priority);
    }

    void MsgDispatchor::wait(msgid id, Message** ppWait)
    {
        //setDirectDispatchor(id, boost::bind(&_waitDispatchor, ppWait, _1), 1);
    }

    void MsgDispatchor::setTaskDispatchor(msgid id, MsgProc proc, int life, ulong worker, int priority)
    {
        setDirectDispatchor(id, boost::bind(&_managedDispatchor, id, proc, worker, priority, _1), life, worker, priority);
    }

}
