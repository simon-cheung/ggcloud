#pragma once
namespace oo{
    class oriole_user;
    class oriole_service        
    {
        std::string mId;
        typedef Hashmap<std::string, oriole_user*> user_map;
        user_map    user_map_;
    public:
        oriole_service(void);
        ~oriole_service(void);

        int startup(const std::string& id, const std::string& cfg);
        int stop();

        size_t calc_cache() const;
        size_t serialize(std::string& is);
        size_t deserialize(const std::string& out);

        const std::string& getId() const{ return mId;}

        int  dispatch_msg(const std::string& from, const std::string& to, const std::string& msg);
    protected:
        int handle_user_login(const std::string& from, oo::proto::user_login* ul);
    public:// service frame
        static void*   creator();
        static void    destory(void* inst);

        static int     startup(void* inst, const char* id, const char* conf);
        static int     stop(void* inst);
        static size_t  serialize(void* inst, void* buf, size_t d_len);
        static size_t  deserialize(void* inst, const void* buf, size_t d_len);

        static const char* getId(void* inst);
        static const char* getModuleName(void* inst) ;

        static int proc_msg(void* delegate, const char* frm, const char* to, void* msg_buf, size_t len);
    };

}