//
//  msg_flow.h
//  oriole
//
//  Created by MacOS on 13-9-12.
//  Copyright (c) 2013年 simon. All rights reserved.
//

#ifndef __oriole__msg_flow__
#define __oriole__msg_flow__

namespace oo {
    struct msg_step{
        std::vector<std::string> from_;
        std::vector<std::string> to_;
        std::vector<Message*>    msg_;
    };
    
    class msg_flow{
        std::string flow_id_;   // local identify in local node
        msg_step    curr_step_;
        msg_step    next_step_;
    public:
        int start(msg_step& st);
        
        int next(msg_step& nx);
        int next_begin();
        int next(std::string& from, std::string& to, Message*);
        int next_end();
        
        int final(); //?
    };
}

#endif /* defined(__oriole__msg_flow__) */
