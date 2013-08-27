#include "ospch.h"

namespace oo{
    netpacket::netpacket()
        : body_(NULL), writepos_(NULL), readpos_(NULL)
    {
    }

    netpacket::netpacket(const netpacket& pkt)
    {
    }

    netpacket& netpacket::operator=(const netpacket& pkt)
    {
    }

    char* netpacket::body()
    {
    }

    // write
    char* netpacket::getWritePos()
    {
        return writepos_;
    }

    int   netpacket::getCacheSize()
    {
        return bodylen_ - (writepos_ - body_);
    }

    bool  netpacket::increaseCacheSize(int size)
    {
    }

    void  netpacket::seekWritePos(int from, int off)
    {
    }

    int   netpacket::write(const void* data, int len)
    {
    }
    // read
    char* netpacket::getReadPos()
    {
    }

    void  netpacket::seekReadPos(int from, int off)
    {
    }

    int   netpacket::read(void* data, int len)
    {
    }
}