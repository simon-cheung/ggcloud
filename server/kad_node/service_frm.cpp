#include "stdafx.h"
#include "o_net_conf.pb.h"
#include "unit_base.h"
#include "service_frm.h"
#include "o_conf.pb.h"

#if OO_PLATFORM == OO_PLATFORM_WIN32
#  define WIN32_LEAN_AND_MEAN
#  if !defined(NOMINMAX) && defined(_MSC_VER)
#	define NOMINMAX // required to stop windows.h messing up std::min
#  endif
#  include <windows.h>
#endif

#if OO_PLATFORM == OO_PLATFORM_APPLE || OO_PLATFORM == OO_PLATFORM_APPLE_IOS
#   include "macUtils.h"
#   include <dlfcn.h>
#endif

#if OO_PLATFORM == OO_PLATFORM_WIN32
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif OO_PLATFORM == OO_PLATFORM_LINUX || OO_PLATFORM == OO_PLATFORM_ANDROID
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )

#elif OO_PLATFORM == OO_PLATFORM_APPLE || OO_PLATFORM == OO_PLATFORM_APPLE_IOS
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) mac_loadDylib( a )
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )

#elif OO_PLATFORM == OO_PLATFORM_SYMBIAN || OO_PLATFORM == OO_PLATFORM_NACL
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) 0
#    define DYNLIB_GETSYM( a, b ) 0
#    define DYNLIB_UNLOAD( a ) 0

#endif

namespace oo{

    typedef service_desc* (*PFN_Definition)();
    void service_frm::startup(const std::string& tbl_conf){
        _load_static_module();
        if(!tbl_conf.empty()){
            oo::conf::module_lst ml;
            google::protobuf::TextFormat::ParseFromString(tbl_conf, &ml);
            for(int i = 0; i < ml.module_file_size(); i++){
                _load_module(ml.module_file(i));
            }
        }
    }

    int service_frm::_load_module(const std::string& file){
        std::string name = file;
#if OO_PLATFORM == OO_PLATFORM_LINUX || OO_PLATFORM == OO_PLATFORM_NACL
        // dlopen() does not add .so to the filename, like windows does for .dll
	if (name.find(".so") == std::string::npos)
           name += ".so";
#elif OO_PLATFORM == OO_PLATFORM_APPLE
        // dlopen() does not add .dylib to the filename, like windows does for .dll
        if (name.substr(name.length() - 6, 6) != ".dylib")
			name += ".dylib";
#elif OO_PLATFORM == OO_PLATFORM_WIN32
		// Although LoadLibraryEx will add .dll itself when you only specify the library name,
		// if you include a relative path then it does not. So, add it to be sure.
		if (name.substr(name.length() - 4, 4) != ".dll")
			name += ".dll";
#endif
        DYNLIB_HANDLE inst = (void*)DYNLIB_LOAD( name.c_str() );

        if( !inst ){
            PFN_Definition pfnd = (PFN_Definition)DYNLIB_GETSYM(inst, "mod_definition");
            if(!pfnd){
                DYNLIB_UNLOAD(inst);
                return -1;
            }
            service_desc* sd = pfnd();
            sd->inst_ptr = inst;
            reg_module(sd);
        }
    }

    int service_frm::_load_static_module(){
        // handly load

        return 0;
    }

    int service_frm::_unload_module(const std::string& file){
        module_map::iterator itf = module_map_.find(file);
        if(itf == module_map_.end())
            return -1;
        const service_desc* sd = itf->second;
        DYNLIB_UNLOAD(sd->inst_ptr);
        module_map_.erase(itf);
        return 0;
    }

    void service_frm::stop(){
    }

    void service_frm::reg_module(const service_desc* sd){
        module_map_.insert(module_map::value_type(sd->module_name, sd));
    }

    int service_frm::_start_service(const std::string& id, const std::string& module, const std::string& conf){
        module_map::iterator itf = module_map_.find(module);
        if(itf == module_map_.end())
            return -1;
        const service_desc* sd = itf->second;
        void* hmodule = sd->pfn_creator();
        int res = sd->pfn_startup(hmodule, id.c_str(), conf.c_str());
        if( res == -1 ){
            sd->pfn_destory(hmodule);
            return -1;
        }// start error
        ser_map_.insert(ser_map::value_type(id, module_inst(hmodule, sd)));
        return 0;
    }

    int service_frm::_stop_service(const std::string& id){
        ser_map::iterator itf = ser_map_.find(id);
        if(itf != ser_map_.end()){
            module_inst& mi = itf->second;
            mi.second->pfn_stop(mi.first);
            mi.second->pfn_destory(mi.first);
            ser_map_.erase(itf);
        }
        return 0;
    }

    int service_frm::_update_module(const std::string& name){
        return 0;
    }
}
