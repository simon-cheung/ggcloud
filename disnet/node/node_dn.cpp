#include "pch_dn.h"
#include <regex>

#include "../protocol/config.pb.h"

#include "conf_dn.h"
#include "service_frm.h"
#include "trans_dn.h"

#include "node_dn.h"

namespace dn{
	int node::startup(conf_pkt pkt){

	}

	int node::stop(){

	}
	
	int node::register_object(base_id id){
		std::lock_guard<std::mutex> sl(obj_set_mutex_);
		obj_set_.insert(id);
		return 0;
	}

	int node::unregister_object(base_id id){
		std::lock_guard<std::mutex> sl(obj_set_mutex_);
		obj_set_.erase(id);
		return 0;
	}

	int node::send_to(base_id from, base_id to, net_pkt pkt){

	}

	bool node::has_session(base_id from, base_id to){
		return match_session(from, to) != nullptr;
	}

	session* node::match_session(base_id from, base_id to){
		base_id id = to;
		std::lock_guard<std::mutex> sl(sess_map_mutex_);
		sess_map::iterator itf;
		if ( itf != sess_map_.end())
			return itf->second;
		// partial match
		base_id root;
		base_id branch;
		//base_id flag;

		size_t bpos = to.find('/');
		size_t fpos = to.find(':');
		root = to.substr(0, bpos);
		if (bpos != std::string::npos)
			branch = to.substr(bpos, fpos);
		//if (fpos != std::string::npos)
		//	flag = to.substr(fpos + 1);
		sess_map::iterator itf = sess_map_.find(root + branch);
		if (itf != sess_map_.end())
			return itf->second;
		itf = sess_map_.find(root);
		if (itf != sess_map_.end())
			return itf->second;
		return nullptr;
	}

	int  node::async_gen_session(base_id from, base_id to, std::function<void(bool)> handler){

	}

	int  node::gen_session(base_id from, base_id to){
		node_desc* nd = conf_->get_node(to);
		if (nd == nullptr){
			nd = conf_->get_proxy_node(to);
			if (nd == nullptr)
				return -1;

		}
	}

	int  node::rem_session(base_id from, base_id to){

	}
}