//
//  trans_mgr.cpp
//  oriole
//
//  Created by MacOS on 13-11-4.
//  Copyright (c) 2013年 simon. All rights reserved.
//
#include "stdafx.h"
#include "trans_mgr.h"

namespace oo{
    
    bool trans_mgr::reg_trans(trans_base* tb){
        mutex::scoped_lock sl(trans_mutex_);
        trans_map::iterator it = trans_map_.find(tb->get_id());
        if(it != trans_map_.end())
            return false;
        trans_map_.insert(trans_map::value_type(tb->get_id(), tb));
        return true;
    }
    
    void trans_mgr::unreg_trans(trans_base* tb){
        mutex::scoped_lock sl(trans_mutex_);
        trans_map::iterator it = trans_map_.find(tb->get_id());
        if(it != trans_map_.end())
            trans_map_.erase(it);
    }
    
    trans_base* trans_mgr::find(const std::string& id){
        mutex::scoped_lock sl(trans_mutex_);
        trans_map::iterator it = trans_map_.find(id);
        if(it != trans_map_.end())
            return it->second;
        return NULL;
        
    }
    
    void trans_mgr::dispatch(const std::string& from, const std::string& to, Message* msg){
        trans_mutex_.lock();
        trans_base* tb = NULL;
        trans_map::iterator it = trans_map_.find(tb->get_id());
        if(it != trans_map_.end())
            tb = it->second;
        
        trans_mutex_.unlock();
        if(tb)
            tb->proc_msg(from, to, msg);
    }
    
    trans_base::trans_base(){
        
    }
    
    trans_base::~trans_base(){
    
    }
    
    void trans_base::commit(){
        trans_mgr::instance().reg_trans(this);
    }
    
    void trans_base::cancel(){
        trans_mgr::instance().unreg_trans(this);
    }
    
    uint64 trans_base::gen_index(){
        static oo::AtomicRef ar_;
        return ar_.ref();
    }
}