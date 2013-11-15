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
        int           kad_net_state_;
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
        
        void async_proc_msg(SessionPtr sess, oo::proto::proxy_pkg* pkg);
        void proc_msg(SessionPtr sess, const std::string& from, const std::string& to, Message*);
    public:
        kad_net* get_kad_net() const { return kad_net_; }
    protected:
        // kad net op
        void publish_self();
        void publish_keyvalue();

        // handler
        void handle_node_active(SessionPtr sess, const std::string& from, const std::string& to, Message*);
        void handle_node_kad_update(SessionPtr sess, const std::string& from, const std::string& to, Message*);
        void handle_store_key(SessionPtr sess, const std::string& from, const std::string& to, Message*);
        void handle_store_key_result(SessionPtr sess, const std::string& from, const std::string& to, Message*);
        void handle_find_node(SessionPtr sess, const std::string& from, const std::string& to, Message*);
        void handle_find_node_result(SessionPtr sess, const std::string& from, const std::string& to, Message*);
        void handle_find_value(SessionPtr sess, const std::string& from, const std::string& to, Message*);
        void handle_find_value_result(SessionPtr sess, const std::string& from, const std::string& to, Message*);
    protected:
        void _active_self();
        void _active_self_result(int state);
        void _active_self_timeout();
    };
}