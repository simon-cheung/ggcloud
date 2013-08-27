#pragma once
#include "MsgDispatchor.h"

namespace oo{

    class kad_node_netunit
        : public MsgDispatchor
    {
    public:
        kad_node_netunit(void);
        ~kad_node_netunit(void);

        static kad_node_netunit& instance();

        void start();
        void stop();

    public:
        // net thread
        static void onPacket(SessionPtr pSession, void* buf, size_t len);
        static void onError(SessionPtr pSession, const boost::system::error_code& e);
    };
}