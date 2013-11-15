#include "stdafx.h"
#include <protocol/o_net_conf.pb.h>
#include <protocol/o_conf.pb.h>
#include "kad_net.h"
#include "kad_net_ctrl.h"
#include "task_mgr.h"
#include "kad_node_proxy.h"
#include "kad_node_netunit.h"
#include "MsgDispatchor.h"

#include "trans/knc_active_self.h"

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

        kad_net_state_ = 0;
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
        kad_net_->find_adjacency(oo::node_id::from_hex_string(lni.id()), kadn);
        if(kadn.empty()){ // self is root node
            _active_self_result(1);
        }else{
            oo::proto::proxy_pkg ppkg;
            oo::proto::node_active na;
            oo::proto::node_info* ni = na.mutable_node();
            ni->CopyFrom(lni);
            std::string np;
            na.SerializeToString(&np);
            ppkg.set_pkg_body(np);
            
            ppkg.set_from(lni.id());
            ppkg.set_pkg_type(Pt_Type_Name(oo::proto::node_active));
            for(std::vector<node_info>::iterator it = kadn.begin(); it != kadn.end(); it++){
                ppkg.set_to(it->id());
                kad_node_netunit::instance().async_send_to(&ppkg);
            }
            tm_add_timer(boost::bind(&kad_net_ctrl::_active_self_timeout, this), 100000, 1, ulong(this));
        }
        
    }

    void kad_net_ctrl::_active_self_timeout(){
        
    }
    void  kad_net_ctrl::async_proc_msg(SessionPtr sess, oo::proto::proxy_pkg* pkg){
        if(pkg->pkg_type() == Pt_Type_Name(oo::proto::node_active)){
            oo::proto::node_active* na = netpacket_2_proto<oo::proto::node_active>(pkg->pkg_body());
            tm_add_task(boost::bind(&kad_net_ctrl::handle_node_active, this, sess, pkg->from(), pkg->to(), na), ulong(this));

        }else if(pkg->pkg_type() == Pt_Type_Name(oo::proto::node_kad_update)){
            oo::proto::node_kad_update* na = netpacket_2_proto<oo::proto::node_kad_update>(pkg->pkg_body());
            tm_add_task(boost::bind(&kad_net_ctrl::handle_node_kad_update, this, sess, pkg->from(), pkg->to(), na), ulong(this));

        }else if(pkg->pkg_type() == Pt_Type_Name(oo::proto::store_key)){
            oo::proto::store_key* na = netpacket_2_proto<oo::proto::store_key>(pkg->pkg_body());
            tm_add_task(boost::bind(&kad_net_ctrl::handle_store_key, this, sess, pkg->from(), pkg->to(), na), ulong(this));

        }else if(pkg->pkg_type() == Pt_Type_Name(oo::proto::store_key_result)){
            oo::proto::store_key_result* na = netpacket_2_proto<oo::proto::store_key_result>(pkg->pkg_body());
            tm_add_task(boost::bind(&kad_net_ctrl::handle_store_key_result, this, sess, pkg->from(), pkg->to(), na), ulong(this));

        }else if(pkg->pkg_type() == Pt_Type_Name(oo::proto::find_node)){
            oo::proto::find_node* na = netpacket_2_proto<oo::proto::find_node>(pkg->pkg_body());
            tm_add_task(boost::bind(&kad_net_ctrl::handle_find_node, this, sess, pkg->from(), pkg->to(), na), ulong(this));

        }else if(pkg->pkg_type() == Pt_Type_Name(oo::proto::find_node_result)){
            oo::proto::find_node_result* na = netpacket_2_proto<oo::proto::find_node_result>(pkg->pkg_body());
            tm_add_task(boost::bind(&kad_net_ctrl::handle_find_node_result, this, sess, pkg->from(), pkg->to(), na), ulong(this));

        }else if(pkg->pkg_type() == Pt_Type_Name(oo::proto::find_value)){
            oo::proto::find_value* na = netpacket_2_proto<oo::proto::find_value>(pkg->pkg_body());
            tm_add_task(boost::bind(&kad_net_ctrl::handle_find_value, this, sess, pkg->from(), pkg->to(), na), ulong(this));

        }else if(pkg->pkg_type() == Pt_Type_Name(oo::proto::find_value_result)){
            oo::proto::find_value_result* na = netpacket_2_proto<oo::proto::find_value_result>(pkg->pkg_body());
            tm_add_task(boost::bind(&kad_net_ctrl::handle_find_value_result, this, sess, pkg->from(), pkg->to(), na), ulong(this));
        }else{
        }
    }

    void kad_net_ctrl::handle_node_active(SessionPtr sess, const std::string& from, const std::string& to, Message*){
    }

    void kad_net_ctrl::handle_node_kad_update(SessionPtr sess, const std::string& from, const std::string& to, Message*){
    }

    void kad_net_ctrl::handle_store_key(SessionPtr sess, const std::string& from, const std::string& to, Message*){
    }

    void kad_net_ctrl::handle_store_key_result(SessionPtr sess, const std::string& from, const std::string& to, Message*){
    }

    void kad_net_ctrl::handle_find_node(SessionPtr sess, const std::string& from, const std::string& to, Message*){
    }

    void kad_net_ctrl::handle_find_node_result(SessionPtr sess, const std::string& from, const std::string& to, Message*){
    }

    void kad_net_ctrl::handle_find_value(SessionPtr sess, const std::string& from, const std::string& to, Message*){
    }

    void handle_find_value_result(SessionPtr sess, const std::string& from, const std::string& to, Message*){
    }
}