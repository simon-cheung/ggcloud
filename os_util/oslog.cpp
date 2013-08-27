#include "ospch.h"

// log4cpp
#include <log4cpp/Category.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/SimpleLayout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/PropertyConfiguratorImpl.hh>
#include <log4cpp/NDC.hh>

namespace oo
{
    void* Log::mRootCategory = NULL;
    void* Log::mStackCategory = NULL;
    Log::Log()
    {
    }

    Log::~Log()
    {
    }

    Log* Log::Instance()
    {
        static Log sInstance;
        return &sInstance;
    }

    int Log::log(void* category, int level, const char* stringFormat, ...)
    {
        if(!category)
        {
            std::cout << "warning, push log to category, no init" << std::endl;
            return 0;
        }
        log4cpp::Category& cate = *((log4cpp::Category*)category);
        if(!cate.isPriorityEnabled(level))
            return 0;
        va_list va;
        va_start(va, stringFormat);
        cate.logva(level, stringFormat, va);
        va_end(va);

        return 1;
    }

	int Log::log(const char* source, const char * functionName,int line, void* category, int level, const char* stringFormat, ...)
    {
        if(!category)
        {
            std::cout << "warning, push log to category, no init" << std::endl;
            return 0;
        }
        log4cpp::Category& cate = *((log4cpp::Category*)category);
        if(!cate.isPriorityEnabled(level))
            return 0;
		log4cpp::EventEx ee;
		ee.sourceName = source;
		ee.functionName = functionName;
		ee.lineNum = line;
        va_list va;
        va_start(va, stringFormat);
        cate.logva2(ee ,level,stringFormat, va);
        va_end(va);

        return 1;
    }

    void* Log::getCategory(const char* category)
    {
        log4cpp::Category& cate = log4cpp::Category::getInstance(category);
        return &cate;
    }

    int Log::log(const char* szCategory, int level, const char* stringFormat, ...)
    {
        void* pcate = getCategory(szCategory);

        if(!pcate)
        {
            std::cout << "warning, push log to category, but no init" << std::endl;
            return 0;
        }
        log4cpp::Category& cate = *((log4cpp::Category*)pcate);
        if(!cate.isPriorityEnabled(level))
            return 0;

        va_list va;
        va_start(va, stringFormat);
        cate.logva(level, stringFormat, va);
        va_end(va);

        return 1;
    }
	int Log::log(const char * source, const char * functionName, int line, const char* szCategory, int level, const char* stringFormat, ...)
    {
        void* pcate = getCategory(szCategory);

        if(!pcate)
        {
            std::cout << "warning, push log to category, but no init" << std::endl;
            return 0;
        }
        log4cpp::Category& cate = *((log4cpp::Category*)pcate);
        if(!cate.isPriorityEnabled(level))
            return 0;

		log4cpp::EventEx ee;
		ee.sourceName = source;
		ee.functionName = functionName;
		ee.lineNum = line;
       
        va_list va;
        va_start(va, stringFormat);
        cate.logva2(ee, level, stringFormat, va);
        va_end(va);

        return 1;
    }

    int Log::end()
    {
        return 0;
    }

    int Log::start(const char* config )
    {
        try 
        { 
            log4cpp::PropertyConfigurator::configure(config);
        } 
        catch(log4cpp::ConfigureFailure& f) 
        {
            std::cout << "!!!! Log Configure Problem " << f.what() << " !!!!!" << std::endl;
            return -1;
        }

        log4cpp::Category& root = log4cpp::Category::getRoot();

        mRootCategory = &root;
        //// configuration
        //// 这些对象即使配置文件没有定义也可以使用，不过其属性继承其父category
        //// log4cpp::Category* root = &log4cpp::Category::getRoot();
        //log4cpp::Category& root = log4cpp::Category::getRoot();
        //log4cpp::Category& sub1 = log4cpp::Category::getInstance(memp::string("sub1"));
        //log4cpp::Category& sub3 = 
        //    log4cpp::Category::getInstance(memp::string("sub1.sub2"));
        //// 3 正常使用这些category对象进行日志处理。
        //// sub1 has appender A1 and rootappender.
        //sub1.info("This is some info");
        //sub1.alert("A warning");

        //// sub3 only have A2 appender.
        //sub3.debug("This debug message will fail to write");
        //sub3.alert("All hands abandon ship");
        //sub3.critStream() << "This will show up << as " << 1 << " critical message" 
        //    << log4cpp::CategoryStream::ENDLINE;
        //sub3 << log4cpp::Priority::P_ERROR 
        //    << "And this will be an error"  
        //    << log4cpp::CategoryStream::ENDLINE;
        //sub3.log(log4cpp::Priority::P_WARN, "This will be a logged warning");


        return 0;

    }

    void Log::startThreadTrace()
    {
        // 开启另一个进程， 记录栈运行状况
        log4cpp::Category& stack = log4cpp::Category::getInstance("__stack");
        stack.setPriority(LL_Info);
        mStackCategory = &stack;
    }

    StackRecord::StackRecord(const char* sz)
    {
        log4cpp::HFrame hf;
        memcpy(hf.v, &(sz), sizeof(sz));
        log4cpp::NDC::push(hf);

        //if(Log::mStackCategory)
        //{
        //    log4cpp::Category& cate = *((log4cpp::Category*)Log::mStackCategory);
        //    cate.log(LL_Info, "%d, enter %s", log4cpp::NDC::getDepth(), sz);
        //}
    }

    StackRecord::~StackRecord()
    {
        /*const char* con = */log4cpp::NDC::pop();
        //if(Log::mStackCategory)
        //{
        //    log4cpp::Category& cate = *((log4cpp::Category*)Log::mStackCategory);
        //    cate.log(LL_Info, "%d, leave %s", log4cpp::NDC::getDepth(), con);
        //}
    }

    void StackRecord::push(const char* sz)
    {
        log4cpp::HFrame hf;
        memcpy(hf.v, &(sz), sizeof(sz));
        log4cpp::NDC::push(hf);
    }

    void StackRecord::pop()
    {
        log4cpp::NDC::pop();
    }

    const char* StackRecord::get()
    {
        return log4cpp::NDC::get();
    }

    void R_push(log4cpp::NDC* p, log4cpp::HFrame& step)
    {
        StackTimeThresholdReporter& reporter = *(StackTimeThresholdReporter*)(p->mlparam);
        if(!reporter.instack)
        {
            const char* name;
            memcpy(&name, step.v, sizeof(name));

            if(name == reporter.start)
            {
                reporter.instack = true;
                uint64 t = GetTimeMsec();
                memcpy(step.v + sizeof(void*), &t, sizeof(t));
                // t==-1,表示为开始执行
                t = -1;
                reporter.result.push_back(StackTimeThresholdReporter::ResultLst::value_type(name, t ));

            }
        }
        else        // instack
        {
            uint64 t = GetTimeMsec();
            memcpy(step.v + sizeof(void*), &t, sizeof(t));

            // t==-1,表示为开始执行
            t = -1;
            const char* name;
            memcpy(&name, step.v, sizeof(name));
            reporter.result.push_back(StackTimeThresholdReporter::ResultLst::value_type(name, t ));
        }
    }

    void R_print(log4cpp::NDC* p)
    {
        StackTimeThresholdReporter& reporter = *(StackTimeThresholdReporter*)(p->mlparam);
        void* root = Log::Instance()->getCategory("");

        Log::Instance()->log(root, LL_Alert, "Stack watch, beyond threshold %d(ms)", reporter.threshold);
        int i = 0;
        for(std::vector<StackTimeThresholdReporter::Result>::iterator it = reporter.result.begin(); it != reporter.result.end();)
        {
            i++;
            if( it->elapsed == -1)
                Log::Instance()->log(root, LL_Alert, "Step %d, Enter [%s]", i, it->stepname);
            else
                Log::Instance()->log(root, LL_Alert, "Step %d, Leave [%s], elapsed time %llu(ms)", i, it->stepname, it->elapsed);

            it++;
        }
    }

    void R_pop(log4cpp::NDC* p, log4cpp::HFrame step)
    {
        StackTimeThresholdReporter& reporter = *(StackTimeThresholdReporter*)(p->mlparam);
        if(reporter.instack)
        {
            const char* name;
            memcpy(&name, step.v, sizeof(name));

            reporter.result.push_back(StackTimeThresholdReporter::ResultLst::value_type(name, *(uint64*)(step.v + sizeof(void*)) ));
            StackTimeThresholdReporter::Result& re = reporter.result.back();

            re.elapsed = GetTimeMsec() - re.elapsed;
            
            if(re.elapsed >= reporter.threshold)
                reporter.bprint = true;
            if(name == reporter.start)
            {
                if(reporter.bprint)
                    R_print(p);

                reporter.bprint = false;
                reporter.instack = false;
                reporter.result.clear();
            }
        }
    }

    StackTimeThresholdReporter::StackTimeThresholdReporter(const uint32& t
            , const char* s
        ) 
        : threshold(t), start(s), instack(false), bprint(false)
    {
            log4cpp::NDC::changeListener(&R_push, &R_pop, this);
    }

    StackTimeThresholdReporter::~StackTimeThresholdReporter()
    {
        log4cpp::NDC::changeListener(NULL, NULL, 0);
    }

}
