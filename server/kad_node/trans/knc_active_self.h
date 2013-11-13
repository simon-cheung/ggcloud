//
//  knc_active_self.h
//  oriole
//
//  Created by MacOS on 13-11-13.
//  Copyright (c) 2013年 simon. All rights reserved.
//

#ifndef __oriole__knc_active_self__
#define __oriole__knc_active_self__

namespace oo {
    class knc_active_self : public trans_base{
        kad_net_ctrl* knc_;
    public:
        bool startup(kad_net_ctrl* knc);
        
        void proc_msg(const std::string& from, const std::string& to, Message* msg)
    };
}

#endif /* defined(__oriole__knc_active_self__) */
