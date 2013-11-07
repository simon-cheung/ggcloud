#pragma once

namespace oo{

    class kad_node_netunit
        : public singleton_default<kad_node_netunit>
    {
        typedef Hashmap<std::string, SessionPtr> session_map;
        session_map session_map_;
        session_map pre_session_map_;
        mutex       session_mutex_;
    public:
        kad_node_netunit(void);
        ~kad_node_netunit(void);

        void start(kad_net* kn);
        void stop();

        void accept_session(std::string id, SessionPtr sess);
        void reject_session(std::string id, SessionPtr sess);
        
        bool async_send_to(oo::proto::proxy_pkg* ppkg);
    protected:
        void handle_conn(SessionPtr pNew, std::string service);
        // net thread
        void waitSession(SessionPtr pSession);
        void onPacket(SessionPtr pSession, void* buf, size_t len);
        void onError(SessionPtr pSession, const boost::system::error_code& e);        
        // worker
        void work_waitSession(SessionPtr pSession);
        void work_onPacket(SessionPtr pSession, Message* msg);
        void work_onError(SessionPtr pSession);  
    };
}