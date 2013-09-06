#include "oriole_pch.h"
#include "oriole_object.h"
#include "oriole_service.h"
#include <protocol/o_s_user.pb.h>
#include <kad_node/unit_base.h>

static service_desc oo_sd = {
    {"oriole"},
    NULL,
    &oo::oriole_service::creator,
    &oo::oriole_service::destory,
    &oo::oriole_service::startup,
    &oo::oriole_service::stop,
    &oo::oriole_service::serialize,
    &oo::oriole_service::deserialize,
    &oo::oriole_service::getId,
    &oo::oriole_service::getModuleName,
    &oo::oriole_service::proc_msg
};
namespace oo{

    oriole_service::oriole_service(void)
    {
    }
    
    oriole_service::~oriole_service(void)
    {
    }

    int oriole_service::startup(const std::string& id, const std::string& cfg){
        mId = id;
    }

    int oriole_service::stop(){
    }

    size_t oriole_service::calc_cache() const{
    }

    size_t oriole_service::serialize(std::string& istream){
    }

    size_t oriole_service::deserialize(const std::string& istream){
    }

    int  oriole_service::proc_msg(Message* msg){
    }

    //=======================================================================================
    // module interface
    void*   oriole_service::creator(){
        return (void*)new oriole_service;
    }

    void    oriole_service::destory(void* inst){
        delete (oriole_service*)inst;
    }

    int     oriole_service::startup(void* inst, const char* id, const char* conf){
        oriole_service* os = (oriole_service*)inst;
        return os->startup(id, conf);
    }

    int     oriole_service::stop(void* inst){
        oriole_service* os = (oriole_service*)inst;
        return os->stop();
    }

    size_t  oriole_service::serialize(void* inst, void* buf, size_t d_len){
        oriole_service* os = (oriole_service*)inst;
        if(buf == NULL){
            return os->calc_cache();
        }else{
            std::string outs;
            os->serialize(outs);
            if(outs.length() > d_len)
                assert(false, "Buffer length, less");
            d_len = outs.length();
            memcpy(buf, outs.c_str(), outs.length());
        }
        return d_len;
    }

    size_t  oriole_service::deserialize(void* inst, const void* buf, size_t d_len){
        oriole_service* os = (oriole_service*)inst;
        std::string is((const char*)buf, d_len);
        return os->deserialize(is);        
    }

    const char* oriole_service::getModuleName(void* inst) {
        return oo_sd.module_name;
    }

    int     oriole_service::proc_msg(void* inst, void* msg_buf, size_t len){
        oriole_service* os = (oriole_service*)inst;
    }
}

OOExport service_desc* mod_definition(){
    return &oo_sd;
}