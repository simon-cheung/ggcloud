#pragma once

namespace oo{

    class kad_net;

    class kad_net_ctrl
        : public singleton_default<kad_net_ctrl>
    {
        kad_net* kad_net_;
        typedef Hashmap<std::string, std::string> key_value_map;
        key_value_map key_value_map_;
    public:
        kad_net_ctrl(void);
        ~kad_net_ctrl(void);

        int startup(const std::string& conf);
        int stop();

        int publish(const std::string& key, const std::string& value, bool bvalid);
        int query(const std::string& key, std::string& value);

        int local_value_query(const std::string& key, std::string& value);
    protected:
        // net thread
        void waitSession(SessionPtr pSession);
        void onPacket(int idt, SessionPtr pSession, void* buf, size_t len);
        void onError(int idt, SessionPtr pSession, const boost::system::error_code& e);
        void onPacketOnSession(SessionPtr pSession, void* buf, size_t len);
        void onErrorOnSeesion(SessionPtr pSession, const boost::system::error_code& e);
    protected:
        // kad net op
        void publish_self();
        void publish_keyvalue();

    protected:
        void handle_conn(SessionPtr pNew, std::string service);
        void _active_self();
    };
}