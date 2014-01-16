#ifndef __service_frm_h__
#define __service_frm_h__

#include <string>
#include <functional>

namespace dn{
	typedef std::string base_id;
	typedef std::string net_pkt;
	typedef const base_id c_base_id;
	typedef const net_pkt c_net_pkt;
	typedef const c_base_id& c_base_id_ref;
	typedef const c_net_pkt& c_net_pkt_ref;

	enum SFRM_OP_CODE {
		GEN_SESSION,
		REM_SESSION,
		SEND,
		SEND_RESULT,
		RECV,
	};

	struct sfrm_net_pkt{
		base_id from_;
		base_id to_;
		net_pkt pkt_;
	};

	struct sfrm_pkt{
		int op_code_;
		int op_params_;

		sfrm_net_pkt net_pkt_;
	};

    class service_frm{
    public:
		virtual int send_to(base_id from, base_id to, net_pkt pkt) = 0;
		virtual int async_request(base_id from, base_id to, net_pkt pkt, sfrm_pkt_handle handler) = 0;
		virtual int async_request(const std::vector<sfrm_net_pkt>& reqs, sfrm_batch_pkt_handle handler) = 0;

		virtual int reg_entry(base_id id, sfrm_pkt_handle entry_pt, int life = -1) = 0;
		virtual int unreg_entry(base_id id) = 0;
    };
}

#endif