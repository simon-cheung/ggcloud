#pragma once
#include "cross_macro.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#define WIN32_LEAN_AND_MEAN
#endif	

// boost
#include <boost/any.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#define Pt_Msg_Id(msg) \
    ((ulong)(msg->GetDescriptor()))
#define Pt_Type_Id(type) \
    ((ulong)(type::descriptor())

#define Pt_Msg_Name(msg) \
    (msg->GetDescriptor()->name())
#define Pt_Type_Name(type) \
    (type::descriptor()->name())
     
#define Pt_Msg_is_Type(msg, type) \
 (Pt_Msg_Id(msg) == Pt_Type_Id(type))


#ifdef WIN32
#include <conio.h>
#include <io.h>
#else
#include <curses.h>
#endif

#include <string>
#include <cstring>
#include <fcntl.h>
#include <cstddef>
#include <algorithm>
#include <functional>
#include <errno.h>
#include <limits.h>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <iostream>
#include <fstream>
#include <exception>
#include <numeric>
#include <stack>

#ifdef _MSC_VER
#include <hash_map>
#include <hash_set>
#define Hashmap stdext::hash_map
#define Hashmultimap stdext::hash_multimap
#define Hashset stdext::hash_set
#define Hashmultiset stdext::hash_multiset
#else
#include <ext/hash_map>
#include <ext/hash_set>
#define Hashmap __gnu_cxx::hash_map
#define Hashmultimap __gnu_cxx::hash_multimap
#define Hashset __gnu_cxx::hash_set
#define Hashmultiset __gnu_cxx::hash_multiset
#endif

namespace oo
{
#ifdef WIN32
    typedef unsigned __int64	uint64;
    typedef __int64				int64;
#else
    typedef unsigned long long  uint64;
    typedef long long           int64;
#endif
    typedef unsigned int        uint32;
    typedef signed   int        int32;

    typedef unsigned short      uint16;
    typedef signed   short      int16;

    typedef unsigned char       uint8;
    typedef signed   char       int8;

    typedef float               float32;
    typedef double              float64;

    typedef void*               pointer;

    typedef unsigned long       ulong;
    // temp, for boost
    typedef boost::thread       thread;
    typedef boost::mutex        mutex;
    typedef boost::recursive_mutex  recursive_mutex;
    typedef boost::condition_variable_any    condition;

	typedef std::string			String;
	typedef std::wstring		WString;

    using boost::uuids::uuid;
    typedef boost::filesystem::path Content;
    using google::protobuf::Message;

    std::string getStringUuid(const uuid& uid);

    struct UUIDObject
    {
        uuid mId;
        std::string mName;

        UUIDObject(){}
        UUIDObject(const uuid& id){ mId = id; mName = getStringUuid(mId); }

        const uuid& getId() const { return mId; }
        const std::string& getName() const { return mName; }
    };
}


#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)

typedef long AtomicWord;
#define ATOMIC_REF(x) _InterlockedIncrement(x)
#define ATOMIC_REL(x) _InterlockedDecrement(x)
#define ATOMIC_SET(x,y) (x=y)

#elif defined(__APPLE_CC__)
#include <libkern/OSAtomic.h>
typedef int64_t AtomicWord;
#define ATOMIC_REF(x) OSAtomicIncrement64(x)
#define ATOMIC_REL(x) OSAtomicDecrement64(x)
#define ATOMIC_SET(x,y) (x=y)

#else
#if __GNUC__ * 100 + __GNUC_MINOR__ >= 402
# include <ext/atomicity.h> 
#else 
# include <bits/atomicity.h>
#endif
typedef _Atomic_word AtomicWord;
#define ATOMIC_REF(x) __gnu_cxx::__exchange_and_add(x, 1) + 1
#define ATOMIC_REL(x)  __gnu_cxx::__exchange_and_add(x, -1) - 1
#define ATOMIC_SET(x,y) x = y
#endif

namespace oo
{
    struct AtomicRef
    {
        mutable AtomicWord mReference;
        AtomicRef() 
        {
            ATOMIC_SET(mReference, 0);
        }

        // 引用计数
        long ref() const 
        { 
            return ATOMIC_REF(&mReference);
        }

        long unref() const 
        {
            return ATOMIC_REL(&mReference);
        }
    };

    struct AtomicRefWith
    {
        AtomicWord* mReference;

        AtomicRefWith() : mReference(NULL){}

        AtomicRefWith(AtomicWord* outref)
            : mReference((AtomicWord*)outref)
        {
            ATOMIC_SET(*mReference, 0);
            ref();
        }

        AtomicRefWith(const AtomicRefWith& arw)
            : mReference(arw.mReference)
        {
            ref();
        }

        AtomicRefWith& operator=(const AtomicRefWith& arw)
        {
            mReference = arw.mReference;
            ref();
        }

        void bind(AtomicWord* outref)
        {
            mReference = outref;
            ATOMIC_SET(*mReference, 0);
        }
        // 引用计数
        long ref() const 
        { 
            if(mReference)
                return ATOMIC_REF(mReference);
            else
                return 0;
        }

        long unref() const 
        {
            if(mReference)
                return ATOMIC_REL(mReference);
            else
                return 0;
        }
    };

    // T must be: no-throw default constructible and no-throw destructible
    template <typename T>
    struct singleton_default
    {
      public:
        typedef T object_type;

        // If, at any point (in user code), singleton_default<T>::instance()
        //  is called, then the following function is instantiated.
        static object_type & instance()
        {
          // This is the object that we return a reference to.
          // It is guaranteed to be created before main() begins because of
          //  the next line.
          static object_type obj;
          return obj;
        }

	    static object_type & Instance()
	    {
		    return instance();
	    }

	    static object_type * instance_ptr()
	    {
		    return &instance();
	    }
    };
}

namespace oo
{
    // protobuf用作配置文档，辅助函数
    // protobuf用作配置文档，辅助函数
    template<class Message>
    inline void write_config_file(const std::string& path, Message& msg)
    {
        std::ofstream ofs(path);
        ofs.open(path, std::ios_base::out | std::ios_base::trunc);
        if (ofs.is_open())
        {
            google::protobuf::io::OstreamOutputStream fo(&ofs);

            google::protobuf::TextFormat::Print(msg, &fo);
        }
    }

    template<class Message>
    inline void read_config_file(const std::string& path, Message& msg)
    {
        std::ifstream  ifi;
        ifi.open(path, std::ios_base::in);
        if (ifi.is_open())
        {
            google::protobuf::io::IstreamInputStream fi(&ifi);
            google::protobuf::TextFormat::Parse(&fi, &msg);
        }
    }
}

namespace oo
{
    class netpacket
        : protected AtomicRefWith
    {
        char* body_;
        char* writepos_;
        char* readpos_;
        int   bodylen_;
    public:
        netpacket();
        netpacket(const netpacket& pkt);
        netpacket& operator=(const netpacket& pkt);
    public:
        char* body();

        // write
        char* getWritePos();
        int   getCacheSize();
        bool  increaseCacheSize(int size);
        void  seekWritePos(int from, int off);
        int   write(const void* data, int len);

        // read
        char* getReadPos();
        void  seekReadPos(int from, int off);
        int   read(void* data, int len);
    };

    inline bool protobufmessage_to_netpacket(const google::protobuf::Message& proto, netpacket* msg)
    {
        int len = msg->getCacheSize();
        int cache = proto.GetCachedSize();
        bool res;

        if( cache > len )
            msg->increaseCacheSize(cache);

        char* buf = msg->getWritePos();
        res = proto.SerializeToArray(buf, cache);
        msg->seekWritePos(SEEK_CUR, cache);
        return res;
    }
}