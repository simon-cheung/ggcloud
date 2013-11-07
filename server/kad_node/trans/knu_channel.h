//
//  knu_channel.h
//  oriole
//
//  Created by MacOS on 13-11-4.
//  Copyright (c) 2013年 simon. All rights reserved.
//

#ifndef oriole_knu_channel_h
#define oriole_knu_channel_h
#include "trans_mgr.h"
#include "o_net_ctrl.h"

namespace oo {
    class knu_channel : public trans_base{
    public:
        SessionPtr peer_;
        std::string peer_id_;
        std::vector<oo::proto::proxy_pkg*> pkg_que_;
        bool startup(const std::string& sid, SessionPtr sess, oo::proto::proxy_pkg* ppkg);
        
        void query_node_result(node_info ni);
        void queue(oo::proto::proxy_pkg* ppkg);
    public:
        virtual void proc_msg(const std::string& from, const std::string& to, Message* msg);
    }
}

#endif
