//
//  trans_mgr.cpp
//  oriole
//
//  Created by MacOS on 13-11-4.
//  Copyright (c) 2013年 simon. All rights reserved.
//
#include "stdafx.h"
#include "knu_channel.h"
#include "kad_net_ctrl.h"

namespace oo{
    
    bool knu_channel::startup(const std::string& sid, SessionPtr sess, oo::proto::proxy_pkg* ppkg){
        if(ppkg->pkg_type() != oo::proto::node_active::descriptor()->name()){
            return false;
        }
        peer_ = sess;
        id_ = sid;
        commit();
        kad_net_ctrl::instance().async_query_node(ppkg->from(), boost::bind(&query_node_result, this, _1));
    }
    
    void knu_channel::query_node_result(node_info ni){
    
    }
    
    void knu_channel::queue(oo::proto::proxy_pkg* ppkg){
        
    }

    void knu_channel::proc_msg(const std::string& from, const std::string& to, Message* msg){
        
    }
}