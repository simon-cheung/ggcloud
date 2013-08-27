#pragma once

namespace oo{

    class kad_net;

    class kad_net_ctrl
        : public singleton_default<kad_net_ctrl>
    {
        kad_net* mKadNet;
    public:
        kad_net_ctrl(void);
        ~kad_net_ctrl(void);

        int startup(const std::string& conf);
        int stop();

        int publish(const node_id& key, const std::string& obj, bool bvalid);

        int tell_obj(const node_id& key, const std::string& msg);
        int multi_tell_obj(std::vector<node_id>& dest, const std::string& msg);

    protected:
        void handler_conn(SessionPtr pNew, std::string service);
        void _active_self();
    };
}