#ifndef __service_frm_impl_h__
#define __service_frm_impl_h_

#include "service_frm.h"

namespace dn{

	typedef std::function<void(sfrm_pkt)> sfrm_pkt_handle;
	typedef std::function<void(std::vector<sfrm_pkt>&)> sfrm_batch_pkt_handle;
	struct entry_desc{
		int life_;
		sfrm_pkt_handle handler_; 
	};

    class service_frm_impl : public service_frm{
		node* root_;
		typedef std::unordered_map<base_id, entry_desc> entry_map;
		typedef std::vector<sfrm_pkt> pkt_que;
		entry_map	entry_map_;
		std::mutex	entry_map_mutex_;

		pkt_que		pkt_que_;
		std::mutex	pkt_que_mutex_;

    public:
		int startup(node* root, c_conf_pkt conf);
		int stop();

		int update();
    public:
		bool try_dispatch(base_id from, base_id to);
		int async_dispatch(base_id from, base_id to, net_pkt pkt);
		int async_dispatch(const sfrm_pkt& sp);

		int dispatch(base_id from, base_id to, net_pkt pkt);
		int dispatch(const sfrm_pkt& sp);

		virtual int send_to(base_id from, base_id to, net_pkt pkt);
		// 
		virtual int request(base_id from, base_id to, net_pkt pkt, sfrm_pkt* resp);
		virtual int async_request(base_id from, base_id to, net_pkt pkt, sfrm_pkt_handle handler);
		virtual int async_request(const std::vector<sfrm_net_pkt>& reqs, sfrm_batch_pkt_handle handler);

		virtual int reg_entry(base_id id, sfrm_pkt_handle entry_pt, int life = -1);
		virtual int unreg_entry(base_id id);
    };
}

#endif