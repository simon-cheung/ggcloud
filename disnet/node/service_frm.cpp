#include "pch_dn.h"
#include "trans_dn.h"
#include "node_dn.h"
#include "service_frm_impl.h"

namespace dn{

	int service_frm_impl::startup(node* root, c_conf_pkt conf){
		root_ = root;
	}

	int service_frm_impl::stop(){

	}

	int service_frm_impl::dispatch(base_id from, base_id to, net_pkt pkt){
		sfrm_pkt sp;
		sp.op_code_ = SFRM_OP_CODE::RECV;
		sp.op_params_ = 0;
		sp.net_pkt_.from_ = from;
		sp.net_pkt_.to_ = to;
		sp.net_pkt_.pkt_ = pkt;

		return dispatch(sp);

	}

	int service_frm_impl::dispatch(const sfrm_pkt& sp){
		std::lock_guard<std::mutex> sl(entry_map_mutex_);
		entry_map::iterator it = entry_map_.find(sp.net_pkt_.to_);
		if (it == entry_map_.end()){
			// error msg routine
			return -1;
		}
		(it->second).handler_(sp);

		// handler life
		if (it->second.life_ != -1){
			it->second.life_--;			
		}
		if (it->second.life_ == 0)
			entry_map_.erase(it);
		return 0;
	}

	static void request_util_handler(base_id from, sfrm_pkt_handle sh, sfrm_pkt pkt){
		pkt.net_pkt_.to_ = from; // remove ":requestxxxxxx"
		sh(pkt);
	}

	int service_frm_impl::async_request(base_id from, base_id to, net_pkt pkt, sfrm_pkt_handle handler){
		base_id transid;
		do{
			std::stringstream ss;
			ss << from << ":request" << rand();
			transid = ss.str();
			if (reg_entry(transid, std::bind(&request_util_handler, from, handler, std::placeholders::_1), 1) == 0)
				break;
		} while (1);

		// do send
		{
			// check session
			if (root_->has_session(from, to) &&
				root_->send_to(from, to, pkt) == 0){
				return 0;
			}
			// connect, then send
			root_->async_gen_session(from, to, [&](bool bsuc){
				if (bsuc){
					root_->send_to(from, to, pkt);
					return;
				}
				else{
					// tellout, error
					unreg_entry(transid);

					sfrm_pkt sp;
					sp.op_code_ = SFRM_OP_CODE::SEND_RESULT;
					sp.op_params_ = -1;
					sp.net_pkt_.to_ = from;
					sp.net_pkt_.from_ = from;
					handler(sp);
				}
			});			
		}

		return 0;
	}

	static void batch_request_util_handler(int num, std::vector<sfrm_pkt>* resp, sfrm_batch_pkt_handle sh, sfrm_pkt pkt){
		resp->push_back(pkt);
		if (resp->size() == num){
			sh(*resp);
			delete resp;
		}
	}

	int service_frm_impl::async_request(const std::vector<sfrm_net_pkt>& reqs, sfrm_batch_pkt_handle handler){
		std::vector<sfrm_pkt>* resp = new std::vector<sfrm_pkt>;
		for (std::vector<sfrm_net_pkt>::const_iterator it = reqs.begin(); it != reqs.end(); it++){
			async_request(it->from_, it->to_,it->pkt_, std::bind(&batch_request_util_handler, reqs.size(), resp, handler, std::placeholders::_1));
		}
		return 0;
	}

	int service_frm_impl::send_to(base_id from, base_id to, net_pkt pkt){
		// check session
		if (root_->has_session(from, to) &&
			root_->send_to(from, to, pkt) == 0){
			return 0;
		}
		// connect, then send
		root_->async_gen_session(from, to, [&](bool bsuc){
			if (bsuc){
				root_->send_to(from, to, pkt);
				return;
			}else{
				// tellout, error
				sfrm_pkt sp;
				sp.op_code_ = SFRM_OP_CODE::SEND_RESULT;
				sp.op_params_ = -1;
				sp.net_pkt_.to_ = from;
				sp.net_pkt_.from_ = from;
				dispatch(sp);
			}
		});

		// end
		return 0;
	}

	int service_frm_impl::reg_entry(base_id id, sfrm_pkt_handle entry_pt, int life){
		std::lock_guard<std::mutex> sl(entry_map_mutex_);
		entry_map::iterator it = entry_map_.find(id);
		if (it == entry_map_.end()){
			entry_desc ed;
			ed.handler_ = entry_pt;
			ed.life_ = life;
			entry_map_.insert(entry_map::value_type(id, ed));
		}
		else
			return -1;
		return 0;

	}

	int service_frm_impl::unreg_entry(base_id id){
		std::lock_guard<std::mutex> sl(entry_map_mutex_);
		entry_map::iterator it = entry_map_.find(id);
		if (it == entry_map_.end())
			entry_map_.erase(it);
		else
			return -1;
		return 0;
	}
}