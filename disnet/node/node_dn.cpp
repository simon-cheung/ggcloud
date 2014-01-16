#include "pch_dn.h"

#include "conf_dn.h"
#include "service_frm.h"
#include "trans_dn.h"

#include "node_dn.h"

namespace dn{
	int node::startup(conf_pkt pkt){

	}

	int node::stop(){

	}
	
	int node::send_to(base_id from, base_id to, net_pkt pkt){

	}

	bool node::has_session(base_id from, base_id to){

	}

	int  node::async_gen_session(base_id from, base_id to, std::function<void(bool)> handler){

	}

	int  node::gen_session(base_id from, base_id to){

	}

	int  node::rem_session(base_id from, base_id to){

	}
}