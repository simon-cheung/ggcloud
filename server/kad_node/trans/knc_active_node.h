//
//  knc_active_node.h
//  oriole
//
//  Created by MacOS on 13-11-5.
//  Copyright (c) 2013年 simon. All rights reserved.
//

#ifndef __oriole__knc_active_node__
#define __oriole__knc_active_node__

namespace oo {
    class knc_active_node : public trans_base{
    public:
        SessionPtr peer_;
        oo::proto::proxy_pkg pkg_que_;
        
        bool startup(SessionPtr sess, oo::proto::proxy_pkg* ppkg);
        
    public:
        virtual void proc_msg(const std::string& from, const std::string& to, Message* msg);
    };
}

#endif /* defined(__oriole__knc_active_node__) */
