#ifndef __node_dn_h__
#define __node_dn_h__

namespace dn{
	class node_net;
	class conf;
	struct session{

	};

    class node{
		typedef std::unordered_map<base_id, session*> sess_map;
		typedef std::unordered_set<base_id>			  obj_set;
		sess_map					sess_map_;
		std::mutex					sess_map_mutex_;
		std::condition_variable_any sess_cond_;

		obj_set						obj_set_;		// local object
		std::mutex					obj_set_mutex_;

		node_net*					node_net_;
		conf*						conf_;
    public:
        int startup(conf_pkt pkt);
        int stop();

	public:
		int register_object(base_id);
		int unregister_object(base_id);

		int send_to(base_id from, base_id to, net_pkt pkt);

		bool has_session(base_id from, base_id to);
		int  async_gen_session(base_id from, base_id to, std::function<void(bool)> handler);
		int  gen_session(base_id from, base_id to);
		int  rem_session(base_id from, base_id to);
	protected:
		session* match_session(base_id from, base_id to);
    };
}
#endif