#include "stdafx.h"
#include "kad_node_netunit.h"
#include "kad_node_proxy.h"
#include "kad_net_ctrl.h"

#include "task_mgr.h"
#include <protocol/o_conf.pb.h>
#include <protocol/o_net_ctrl.pb.h>

namespace oo{
    //=============================================================================================
    void kad_node_proxy::start(const std::string& conf)
    {
        oo::conf::proxy_conf pc;
        google::protobuf::TextFormat::ParseFromString(conf, &pc);
        Joint::instance().listenAt(pc.addr().c_str(), pc.port().c_str(), 
            boost::bind(&kad_node_proxy::handler_conn, _1, _2));
    }

    void kad_node_proxy::stop()
    {
    }

    PSession* kad_node_proxy::_genData(){
        if(mSessData.empty()){
            return new PSession;
        }else{
            PSession* ps = mSessData.front();
            mSessData.pop_front();
            return ps;
        }
    }

    void kad_node_proxy::_reuseData(PSession* ps){
        mSessData.push_back(ps);
    }

    int kad_node_proxy::deliver_msg(const std::string& from, const std::string& to, const std::string& msg){

    }
    
    void kad_node_proxy::handler_conn(SessionPtr pNew, std::string /*service*/){
        PSession* ps = kad_node_proxy::instance()._genData();
        ps->connected_t = time(NULL);
        ps->checked_t = 0;
        ps->recv_in_size = 0;
        ps->send_out_size = 0;
        pNew->setData(ps);
        kad_node_proxy::instance().mSess.insert(pNew); // wait check
    }

    void kad_node_proxy::onPacket(SessionPtr pSession, void* buf, size_t len)
    {
        oo::proto::proxy_pkg ppkg;
        if(!ppkg.ParseFromArray(buf, len)){
            pSession->close();
            onError(pSession, boost::system::error_code());
            return;
        }// end, error msg

        PSession* ps = (PSession*)pSession->getData();
        ps->checked_t = time(NULL);
        ps->recv_in_size += len;
        if(ps->peer_id.empty()){
            ps->peer_id = ppkg.from();
            mSessPeer.insert(Hashmap<std::string, SessionPtr>::value_type(ps->peer_id, pSession);
        }else if(ps->peer_id != ppkg.from()){
            pSession->close();
            onError(pSession, boost::system::error_code());
            return;
        }
        kad_net_ctrl::instance().deliver_msg(ppkg.from(), ppkg.to(), ppkg.pkg());
    }

    void kad_node_proxy::onError(SessionPtr pSession, const boost::system::error_code& e)
    {
        std::set<SessionPtr>::iterator itf = kad_node_proxy::instance().mSess.find(pSession);
        if(itf != kad_node_proxy::instance().mSess.end()){
            PSession* ps = (PSession*)pSession->getData();
            pSession->setData(NULL);
            kad_node_proxy::instance()._reuseData(ps);
            kad_node_proxy::instance().mSess.erase(itf);
        }else{
        }
    }
}
