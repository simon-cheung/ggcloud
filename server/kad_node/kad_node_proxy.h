#pragma once

namespace oo{

    struct PSession{
        std::string cert;
        std::string peer_id;
        time_t      connected_t;
        time_t      checked_t;
        uint64      recv_in_size;
        uint64      send_out_size;
    };

    class kad_node_proxy
        : public singleton_default<kad_node_proxy>
    {
        std::set<SessionPtr>    mSess;
        Hashmap<std::string, SessionPtr> mSessPeer;
        std::list<PSession*>    mSessData;
    public:
        kad_node_proxy(void);
        ~kad_node_proxy(void);

        void start(const std::string& conf);
        void stop();

        int deliver_msg(const std::string& from, const std::string& to, const std::string& msg);
    protected:
        PSession* _genData();
        void _reuseData(PSession*);
    public:
        // net thread
        static void handler_conn(SessionPtr pNew, std::string service);
        static void onPacket(SessionPtr pSession, void* buf, size_t len);
        static void onError(SessionPtr pSession, const boost::system::error_code& e);
    };
}