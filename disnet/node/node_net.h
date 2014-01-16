#ifndef __node_net_h__
#define __node_net_h__

namespace dn{

	class node_net{
	public:
		int startup(conf_pkt pkt);
		int stop();
	};
}
#endif