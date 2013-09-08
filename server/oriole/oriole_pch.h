// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif	

#define OO_NONCLIENT_BUILD  // buld dll

#include <osutils.h>
#include <logmacro.h>
#include <bigint.h>

#ifdef OO_NONCLIENT_BUILD
#define OOExport _ooExport
#else
#define OOExport
#endif

namespace oo{

    template<class ProtoMsg>
    bool protobuf_2_netpacket(ProtoMsg* msg, std::string& buff){
        std::string fname = msg->GetTypeName();
        buff = fname;
        buff += '\0';
        msg->SerializeToString(&buff);
        return true;
    }

    inline bool netpacket_2_protobuf(Message** msg, const std::string& buff){
        try{
            size_t zpos = buff.find_first_of('\0');
            std::string m_name = buff.substr(0, zpos);
            const ::google::protobuf::Descriptor* m_desc = ::google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(m_name);
            const Message* m_type = ::google::protobuf::MessageFactory::GetPrototype(m_desc);
            *msg = m_type->New();
            return (*msg)->ParseFromString(buff.substr(zpos + 1));
        }catch(...){
        }
        return false;
    }
}

