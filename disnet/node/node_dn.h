#ifndef __node_dn_h__
#define __node_dn_h__

namespace dn{

	struct session{

	};

    class node{
		typedef std::unordered_map<base_id, session> sess_map;
		sess_map					sess_map_;
		std::mutex					sess_map_mutex_;
		std::condition_variable_any sess_cond_;

    public:
        int startup(conf_pkt pkt);
        int stop();

	public:
		int send_to(base_id from, base_id to, net_pkt pkt);

		bool has_session(base_id from, base_id to);
		int  async_gen_session(base_id from, base_id to, std::function<void(bool)> handler);
		int  gen_session(base_id from, base_id to);
		int  rem_session(base_id from, base_id to);
    };
}
#endif