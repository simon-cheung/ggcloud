#include "stdafx.h"
#include <protocol/o_net_conf.pb.h>
#include <protocol/o_conf.pb.h>
#include "MsgDispatchor.h"
#include "kad_net.h"
#include "kad_net_ctrl.h"
#include "kad_node_netunit.h"
#include "task_mgr.h"

namespace oo{
    static void sendMsg(SessionPtr pSession, uuid fid, uuid toid, std::string msg);
    //=============================================================================================
    kad_node_netunit::kad_node_netunit(){
    }

    kad_node_netunit::~kad_node_netunit(){
    }

    void kad_node_netunit::start(kad_net* kn)
    {
        const oo::proto::node_info& lni = kn->get_local_node_info();
        std::stringstream ss;
        ss << lni.port();

        Joint::instance().listenAt(lni.addr().c_str(), ss.str().c_str(), 
            boost::bind(&kad_node_netunit::handle_conn, kad_node_netunit::instance_ptr(), _1, _2));
    }

    void kad_node_netunit::stop()
    {
    }

    void kad_node_netunit::handle_conn(SessionPtr pNew, std::string service){
        // MsgPortTaskManager
        // timeout 10 seconds
        KDInfo("New Connection [%s] from service[%s]", pNew->getName().c_str(), service.c_str());
        int idt = tm_add_timer(boost::bind(&kad_node_netunit::work_waitSession, pNew), 10000, 1, (ulong)(this));
        // mark
        pNew->setData(NULL);
        // check routine
        pNew->setHandler(boost::bind(&kad_node_netunit::onPacket,  _1, _2, _3), boost::bind(&kad_node_netunit::onError, _1, _2));
    }

    void kad_node_netunit::onPacket(SessionPtr pSession, void* buf, size_t len){
        Message* msg;
        oo::proto::proxy_pkg ppkg;
        if(!ppkg.ParseFromArray(buf, len)){
            KDError("Connection [%s] , check msg error", pSession->getName().c_str());
            pSession->close();
            onError(pSession, boost::system::error_code());
        }// end, error msg
        pSession->setData(1);
        // check node
        kad_net_ctrl::instance().async_query_node(ppkg.from(), );
    }

    void kad_node_netunit::onError(SessionPtr pSession, const boost::system::error_code& e){
        tm_add_task(boost::bind(&kad_node_netunit::work_onError, pSession), (ulong)(this));
    }

    void kad_node_netunit::work_waitSession(SessionPtr pSession){
        if(pSession->getData() != NULL){
            return; // has check
        }
        KDError("Connection [%s] , timeout for check.so close", pSession->getName().c_str());
        pSession->close();
    }
}
