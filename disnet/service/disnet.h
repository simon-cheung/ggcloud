#ifndef __disnet_h__
#define __disnet_h__

#include <string>
#include <functional>


#ifdef DISNET_CORE_EXPORTS
#define DISNET_CORE_API __declspec(dllexport)
#else
#define DISNET_CORE_API __declspec(dllimport)

#endif

namespace dn{
    typedef std::string base_id;
    typedef std::string net_pkt;
    // msg
    DISNET_CORE_API int send_to(base_id from, base_id to, const net_pkt& pkt);
    DISNET_CORE_API int request(base_id from, base_id to, const net_pkt& pkt, net_pkt& resp);

    DISNET_CORE_API int reg_entry(base_id id, std::function<void (net_pkt)> entry_pt);
    DISNET_CORE_API int unreg_entry(base_id id);
}
#endif