#include "stdafx.h"
#include <protocol/o_net_conf.pb.h>
#include <protocol/o_conf.pb.h>
#include "kad_net.h"
#include "kad_net_ctrl.h"
#include "task_mgr.h"
#include "kad_node_proxy.h"
#include "kad_node_netunit.h"
#include "MsgDispatchor.h"

namespace oo{

    kad_net_ctrl::kad_net_ctrl(void)
    {
    }

    kad_net_ctrl::~kad_net_ctrl(void)
    {
    }

    int kad_net_ctrl::startup(const std::string& conf){
        kad_net_ = new kad_net;
        kad_net_->load_conf(conf);

        tm_add_task(boost::bind(&kad_net_ctrl::_active_self, this), (ulong)(this));
        return 1;
    }

    int kad_net_ctrl::stop(){
        return 0;
    }

    int kad_net_ctrl::async_publish(const std::string& key, const std::string& value, bool bvalid, 
                    boost::function<void (int)> result_cb){
        //mutex::scoped_lock sl(key_value_mutex_);
        //if(bvalid){
        //    key_value_map_.insert(key_value_map::value_type(key, value));
        //}else{
        //    key_value_map::iterator itf = key_value_map_.find(key);
        //    if(itf != key_value_map_.end())
        //        key_value_map_.erase(itf);
        //}
        // new publish transaction
        return 0;
    }

    int kad_net_ctrl::async_query(const std::string& key, boost::function<void (std::string)> result_cb){
        std::string value = local_value_query(key);
        if(value.empty()){
            tm_add_task(boost::bind(result_cb, value));
            return 0;
        }
        // else ask to net
    }

    std::string kad_net_ctrl::local_value_query(const std::string& key){
        mutex::scoped_lock sl(key_value_mutex_);
        std::string value;
        key_value_map::iterator itf = key_value_map_.find(key);
        if(itf != key_value_map_.end()){
            value = itf->second;
            return value;
        }
        return "";
    }

    void kad_net_ctrl::_active_self(){
        const oo::proto::node_info& lni = kad_net_->get_local_node_info();
        std::vector<node_info> kadn;
        kad_net_->find_shortest(oo::node_id::from_hex_string(lni.id()), kadn);
        for(std::vector<node_info>::iterator it = kadn.begin(); it != kadn.end(); it++){

        }
    }
}