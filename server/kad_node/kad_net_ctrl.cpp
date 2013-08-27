#include "stdafx.h"
#include "kad_net.h"
#include "kad_net_ctrl.h"
#include "o_net_conf.pb.h"
#include "task_mgr.h"

namespace oo{

    kad_net_ctrl::kad_net_ctrl(void)
    {
    }

    kad_net_ctrl::~kad_net_ctrl(void)
    {
    }

    int kad_net_ctrl::startup(const std::string& conf){
        mKadNet = new kad_net;
        mKadNet->load_conf(conf);
        const node_info& lni = mKadNet->get_local_node_info();
        std::stringstream ss;
        ss << lni.port();
        Joint::instance().listenAt(lni.addr().c_str(), ss.str().c_str(), 
            boost::bind(kad_net_ctrl::handler_conn, kad_net_ctrl::instance_ptr(), _1, _2));

        MsgPortTaskManager::instance().addTask(boost::bind(&_active_self, this), (ulong)(this));
        return 1;
    }

    int kad_net_ctrl::stop(){
    }

    int kad_net_ctrl::publish(const node_id& key, const std::string& obj, bool bvalid){
    }

    int kad_net_ctrl::tell_obj(const node_id& key, const std::string& msg){
    }

    int kad_net_ctrl::multi_tell_obj(std::vector<node_id>& dest, const std::string& msg){
    }

    void kad_net_ctrl::handler_conn(SessionPtr pNew, std::string service){
        Joint::instance().addSession(pNew);
    }

    void kad_net_ctrl::_active_self(){
        const node_info& lni = mKadNet->get_local_node_info();
        std::vector<node_info> kadn;
        mKadNet->find_shortest(node_id::from_hex_string(lni.id()), kadn);
    }

}