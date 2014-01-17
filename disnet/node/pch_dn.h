#ifndef __pch_dn_h__
#define __pch_dn_h__

#include <string>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include <sstream>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/descriptor_database.h>

namespace dn{
    typedef std::string conf_pkt;
	typedef const conf_pkt& c_conf_pkt;
	typedef google::protobuf::Message form_msg;
}

#endif