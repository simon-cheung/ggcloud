#pragma once

typedef struct _service_desc{    
    char    module_name[256];
    void*   inst_ptr;
    void*   (*pfn_creator)();
    void    (*pfn_destory)(void* inst);

    int     (*pfn_startup)(void* inst, const char* id, const char* conf);
    int     (*pfn_stop)(void* inst);
    size_t  (*pfn_serialize)(void* inst, void* buf, size_t d_len);
    size_t  (*pfn_deserialize)(void* inst, const void* buf, size_t d_len);

    const char* (*pfn_getId)(void* inst);
    const char* (*pfn_getModuleName)(void* inst) ;
}service_desc, *service_desc_ptr;

// interface
int send_msg(const char* from, const char* to, void* msg_buf, size_t len);
int reg_dispatchor(const char* id, void* delegate, 
            int (*pfn_dispatchor)(void* delegate, const char* frm, const char* to, void* msg_buf, size_t len));
int unreg_dispatchor(const char* id);
