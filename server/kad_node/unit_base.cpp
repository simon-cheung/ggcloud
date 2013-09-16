#include "stdafx.h"
#include "unit_base.h"
#include "service_frm.h"

int send_msg(const char* from, const char* to, void* msg_buf, size_t len){
    return 0;
}

static int cpp_dis(void* delegate, int (*pfn_dispatchor)(void* delegate, const char* frm, const char* to, const void* msg_buf, size_t len), 
                    const std::string& from, const std::string& to, const std::string& msg)
{
    return pfn_dispatchor(delegate, from.c_str(), to.c_str(), (const void*)msg.c_str(), msg.length());
}

int reg_dispatchor(const char* id, void* delegate, int (*pfn_dispatchor)(void* delegate, const char* frm, const char* to, const void* msg_buf, size_t len)){
    oo::service_frm::instance().setTaskDispatchor(id, boost::bind(&cpp_dis, delegate, pfn_dispatchor, _1, _2, _3));
    return 0;
}

int unreg_dispatchor(const char* id){
    oo::service_frm::instance().remDispatchor(id);
    return 0;
}
