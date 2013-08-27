#pragma once
namespace oo
{
    #ifdef WIN32
    //毫秒
    inline uint64 GetTimeMsec()
    {
        return GetTickCount();
	    /*uint64 time;
	    FILETIME pt;
        GetSystemTimeAsFileTime(&pt);
	    return ((int64)pt.dwHighDateTime)*1000 + pt.dwLowDateTime/1000;*/
    }
    inline uint32 GetCurThreadId()
    {
        return (uint32)::GetCurrentThreadId();
    }
    inline uint32 GetProcessId()
    {
        return (uint32)::GetCurrentProcessId();
    }
    #else //linux
    inline uint64 GetTimeMsec()
    {
	    timeval pt;
	    gettimeofday( &pt, 0 );
	    return ((int64)pt.tv_sec)*1000 + pt.tv_usec/1000;
    }
    inline uint32 GetCurThreadId()
    {
        return (uint32)pthread_self();
    }
    inline uint32 GetProcessId()
    {
        return (uint32)::getpid();
    }
    #endif //end win32 linux

    struct DefineCategory
    {
        void* pCategory;
        const char* szCategory;

        DefineCategory(const char* sz);
    };

    struct StackRecord
    {
        StackRecord(const char* sz);
        ~StackRecord();

        static void push(const char* sz);
        static void pop();
        static const char* get();
    };

    /**
     * Predefined Levels of Priorities. These correspond to the
     * priority levels used by syslog(3).
     **/
    // and ,see log4cpp::Priority class
    enum 
    {
        LL_Emerg  = 0, 
        LL_Fatal  = 0,
        LL_Alert  = 100,
        LL_Crit   = 200,
        LL_Error  = 300, 
        LL_Warn   = 400,
        LL_Notice = 500,
        LL_Info   = 600,
        LL_Debug  = 700,
        LL_Notset = 800
    };

    class Log
    {
        Log();
        ~Log();
    public:
    public:
        static Log* Instance();	
        static void*    mRootCategory;
        static void*    mStackCategory;

        int start(const char* config="../config/Log.conf");
        void startThreadTrace();
        int end();

        void* getCategory(const char* category);
		int log(void* category, int level, const char* stringFormat, ...);
        int log(const char* category, int level, const char* stringFormat, ...);
		//支持打印行号和文件名
        int log(const char* soruce, const char * functionName,int line, void* category, int level, const char* stringFormat, ...);
        int log(const char* soruce, const char * functionName,int line, const char* category, int level, const char* stringFormat, ...);
    };


    inline DefineCategory::DefineCategory(const char* sz)
    {
        szCategory = sz;
        pCategory = Log::Instance()->getCategory(szCategory);
    }
}

namespace oo
{
    struct StackTimeThresholdReporter
    {
        struct Result
        {
            const char* stepname;
            uint64      elapsed;

            Result(const char* p, const uint64& e) : stepname(p), elapsed(e){}
        };
        typedef std::vector<Result> ResultLst;
        ResultLst         result;
        uint32      threshold;             // 如果某一步执行时间超过阀值，则报告
        const char* start;                 // 监视开始step

        bool        instack;
        bool        bprint;

        StackTimeThresholdReporter(const uint32& t
            , const char* start
            );
        ~StackTimeThresholdReporter();
    };
}


