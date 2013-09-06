#pragma once

namespace oo{

    class service_frm 
        : public singleton_default<service_frm>
    { 
        typedef Hashmap<std::string, const service_desc*> module_map;
        typedef std::pair<void*, const service_desc*> module_inst;
        typedef Hashmap<std::string, module_inst> ser_map;
        module_map module_map_;
        ser_map ser_map_;
    public:
        void startup(const std::string& tbl_conf);
        void stop();
        void reg_module(const service_desc* sd);

        int start_service(const std::string& id, const std::string& module, const std::string& conf);
        int stop_service(const std::string& id);
        int update_module(const std::string& name);

        int deliver_msg(const std::string& from, const std::string& to, const std::string& msg);
    protected:
        int _load_static_module();
        int _load_module(const std::string& file);
        int _unload_module(const std::string& file);

        int _start_service(const std::string& id, const std::string& module, const std::string& conf);
        int _stop_service(const std::string& id);
        int _update_module(const std::string& name);
    };
}