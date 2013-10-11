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
        const oo::proto::node_info& lni = kad_net_->get_local_node_info();
        std::stringstream ss;
        ss << lni.port();

        Joint::instance().listenAt(lni.addr().c_str(), ss.str().c_str(), 
            boost::bind(&kad_net_ctrl::handle_conn, kad_net_ctrl::instance_ptr(), _1, _2));

        MsgPortTaskManager::instance().addTask(boost::bind(&kad_net_ctrl::_active_self, this), (ulong)(this));
        return 1;
    }

    int kad_net_ctrl::stop(){
        return 0;
    }

    int kad_net_ctrl::publish(const std::string& key, const std::string& value, bool bvalid){
        if(bvalid){
            key_value_map_.insert(key_value_map::value_type(key, value));
        }else{
            key_value_map::iterator itf = key_value_map_.find(key);
            if(itf != key_value_map_.end())
                key_value_map_.erase(itf);
        }
        return 0;
    }

    int kad_net_ctrl::query(const std::string& key, std::string& value){
        if(local_value_query(key, value) == 0)
            return 0;
        // else ask to net
    }

    int kad_net_ctrl::local_value_query(const std::string& key, std::string& value){
        key_value_map::iterator itf = key_value_map_.find(key);
        if(itf != key_value_map_.end()){
            value = itf->second;
            return 0;
        }
        return 1;
    }

    void kad_net_ctrl::_active_self(){
        const oo::proto::node_info& lni = kad_net_->get_local_node_info();
        std::vector<node_info> kadn;
        kad_net_->find_shortest(oo::node_id::from_hex_string(lni.id()), kadn);
        for(std::vector<node_info>::iterator it = kadn.begin(); it != kadn.end(); it++){

        }
    }

    void kad_net_ctrl::handle_conn(SessionPtr pNew, std::string service){
        // MsgPortTaskManager
        // timeout 10 seconds
        KDInfo("New Connection [%s] from service[%s]", pNew->getName().c_str(), service.c_str());
        int idt = MsgPortTaskManager::instance().addTimer(boost::bind(&kad_net_ctrl::work_waitSession, pNew), 10000, 1, (ulong)(this));
        // mark
        pNew->setData(NULL);
        // check routine
        pNew->setHandler(boost::bind(&kad_net_ctrl::onPacket,  _1, _2, _3), boost::bind(&kad_net_ctrl::onError, _1, _2));
    }


    void kad_net_ctrl::onPacket(SessionPtr pSession, void* buf, size_t len){
        Message* msg;
        if(!netpacket_2_protobuf(&msg, std::string((const char*)buf, len))){
            KDError("Connection [%s] , check msg error", pSession->getName().c_str());
            pSession->close();
            onError(pSession, boost::system::error_code());
        }// end, error msg

        // process msg
        MsgPortTaskManager::instance().addTask(boost::bind(&kad_net_ctrl::work_onPacket, pSession, msg), (ulong)(this));
    }

    void kad_net_ctrl::onError(SessionPtr pSession, const boost::system::error_code& e){
        MsgPortTaskManager::instance().addTask(boost::bind(&kad_net_ctrl::work_onError, pSession), (ulong)(this));
    }
    
    void kad_net_ctrl::work_waitSession(SessionPtr pSession){
        if(pSession->getData() != NULL){
            return; // has check
        }
        KDError("Connection [%s] , timeout for check.so close", pSession->getName().c_str());
        pSession->close();
    }
    void kad_net_ctrl::work_onPacket(SessionPtr pSession, Message* msg){        
        // process msg
    }
    
    void kad_net_ctrl::work_onError(SessionPtr pSession){
    }
}