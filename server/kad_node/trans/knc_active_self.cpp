//
//  knc_active_self.cpp
//  oriole
//
//  Created by MacOS on 13-11-13.
//  Copyright (c) 2013年 simon. All rights reserved.
//
#include "stdafx.h"

#include <protocol/o_net_conf.pb.h>
#include <protocol/o_conf.pb.h>
#include "kad_net.h"
#include "kad_net_ctrl.h"
#include "task_mgr.h"
#include "MsgDispatchor.h"

#include "knc_active_self.h"

namespace oo{

    bool knc_active_self::startup(kad_net_ctrl* knc){

        
    }
    
    void knc_active_self::proc_msg(const std::string& from, const std::string& to, Message* msg){
        
    }
    
}
