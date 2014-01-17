#ifndef __conf_dn_h__
#define __conf_dn_h__

namespace dn{
	class conf{
	public:
		int load(conf_pkt conf);
		int save(std::string path);
	public:
		node_desc* get_node(base_id id);
		node_desc* get_proxy_node(base_id id);
	};
}

#endif