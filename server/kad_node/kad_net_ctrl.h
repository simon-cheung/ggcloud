#pragma once

namespace oo{

    class kad_net;

    class kad_net_ctrl
        : public singleton_default<kad_net_ctrl>
    {
        kad_net* kad_net_;
        typedef Hashmap<std::string, std::string> key_value_map;
        key_value_map key_value_map_;
        mutex         key_value_mutex_;
    public:
        kad_net_ctrl(void);
        ~kad_net_ctrl(void);

        int startup(const std::string& conf);
        int stop();

        int async_publish(const std::string& key, const std::string& value, bool bvalid, 
                    boost::function<void (int)> result_cb);
        int async_query(const std::string& key, 
                    boost::function<void (std::string)> result_cb);

        int async_query_node(const std::string& key, boost::function<void (node_info)> result_cb);

        std::string local_value_query(const std::string& key);
        
        void proc_msg(SessionPtr sess, oo::proto::proxy_pkg* pkg);
    protected:
        // kad net op
        void publish_self();
        void publish_keyvalue();

    protected:
        void _active_self();
    };
}