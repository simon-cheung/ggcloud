#include "stdafx.h"
#include "kad_node_netunit.h"
#include "task_mgr.h"

namespace oo{
    static void sendMsg(SessionPtr pSession, uuid fid, uuid toid, std::string msg);
    //=============================================================================================
    kad_node_netunit& kad_node_netunit::instance()
    {
        static kad_node_netunit __Inst__;
        return __Inst__;
    }

    void kad_node_netunit::start()
    {
        Joint::instance().visit(boost::bind(&oo::kad_node_netunit::onPacket, _1, _2, _3),
            boost::bind(&oo::kad_node_netunit::onError, _1, _2) );
    }

    void kad_node_netunit::stop()
    {
    }

    void kad_node_netunit::onPacket(SessionPtr pSession, void* buf, size_t len)
    {
        Message* msg;
        if(!netpacket_2_protobuf(&msg, std::string((const char*)buf, len))){
            pSession->close();
            onError(pSession, boost::system::error_code());
        }// end, error msg
        kad_node_netunit::instance().dispatch(msg->GetTypeName(), msg);
    }

    void kad_node_netunit::onError(SessionPtr pSession, const boost::system::error_code& e)
    {
        Joint::instance().removeSession(pSession);
    }
}
