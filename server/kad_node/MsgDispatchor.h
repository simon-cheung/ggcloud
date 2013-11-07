#pragma once

namespace oo{
    struct Dispatchor_base;
    typedef boost::function< void (const std::string& from, const std::string& to, Message*)>       MsgProc;
    typedef boost::function_base MsgProcBase;

    class MsgDispatchor
        : public singleton_default<MsgDispatchor>{
        struct Dispatchor
        {
            MsgProc func;
            ulong   worker;
            int     priority;
            int     life;
        };
        typedef Hashmap<std::string, Dispatchor > MsgProcTbl;
        MsgProcTbl        mDispatchors;    
        mutex             mDispatchMutex;
    public:
        // for process
        void wait(const std::string& id, std::string& ppWait);
        // dispatch
        void     setTaskDispatchor(const std::string& id, MsgProc proc, int life = -1, ulong worker = -1, int priority = -1);
        void     remDispatchor(const std::string& id);

        void    lockDispatch();
        void    unlockDispatch();
        bool    tryLockDispatch();

        bool    dispatch(const std::string& from, const std::string& to, Message* msg);
    protected:
        int    _dispatch(const std::string& from, const std::string& to, Message* msg, Dispatchor& da);
    };

    
    struct SNMSession{
        std::string cert;
        std::string peer_id;
        time_t      connected_t;
        time_t      checked_t;
        uint64      recv_in_size;
        uint64      send_out_size;
    };

    template<class ProtoMsg>
    bool protobuf_2_netpacket(ProtoMsg* msg, std::string& buff){
        std::string fname = msg->GetTypeName();
        buff = fname;
        buff += '\0';
        msg->SerializeToString(&buff);
        return true;
    }
    
    inline bool netpacket_2_protobuf(Message** msg, const std::string& buff){
        try{
            size_t zpos = buff.find_first_of('\0');
            std::string m_name = buff.substr(0, zpos);
            const ::google::protobuf::Descriptor* m_desc = ::google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(m_name);
            const Message* m_type = ::google::protobuf::MessageFactory::generated_factory()->GetPrototype(m_desc);
            *msg = m_type->New();
            return (*msg)->ParseFromString(buff.substr(zpos + 1));
        }catch(...){
        }
        return false;
    }
}
