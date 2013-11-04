//
//  trans_mgr.h
//  oriole
//
//  Created by MacOS on 13-11-4.
//  Copyright (c) 2013年 simon. All rights reserved.
//

#ifndef __oriole__trans_mgr__
#define __oriole__trans_mgr__

namespace oo{
    class trans_base;
    class trans_mgr
        : public singleton_default<trans_mgr>
    {
        typedef Hashmap<std::string, trans_base*> trans_map;
        trans_map trans_map_;
        mutex     trans_mutex_;
        friend class trans_base;
    public:
        bool reg_trans(trans_base* tb);
        void unreg_trans(trans_base* tb);
        
        trans_base* find(const std::string& id);
        void dispatch(const std::string& from, const std::string& to, Message* msg);
    };
    
    class trans_base{
    protected:
        std::string id_;
    public:
        trans_base();
        virtual ~trans_base();
        
        const std::string& get_id() const { return id_;}
        
        void commit();
        void cancel();
        virtual void proc_msg(const std::string& from, const std::string& to, Message* msg) = 0;
    };
}

#endif /* defined(__oriole__trans_mgr__) */
