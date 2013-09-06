#pragma once
namespace oo{

    class oriole_service        
    {
        std::string mId;
    public:
        oriole_service(void);
        ~oriole_service(void);

        int startup(const std::string& id, const std::string& cfg);
        int stop();

        size_t calc_cache() const;
        size_t serialize(std::string& is);
        size_t deserialize(const std::string& out);

        const std::string& getId() const{ return mId;}

        int  proc_msg(Message* msg);

    public:// service frame
        static void*   creator();
        static void    destory(void* inst);

        static int     startup(void* inst, const char* id, const char* conf);
        static int     stop(void* inst);
        static size_t  serialize(void* inst, void* buf, size_t d_len);
        static size_t  deserialize(void* inst, const void* buf, size_t d_len);

        static const char* getId(void* inst);
        static const char* getModuleName(void* inst) ;

        static int     proc_msg(void* inst, void* msg_buf, size_t len);

    };

}