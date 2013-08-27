#pragma once

#include "oslog.h"

#define DECLARE_LOG_CATEGORY(name)              \
namespace oo{                                  \
    extern DefineCategory __##name##_Category;      \
}

#define DEFINE_LOG_CATEGORY(name)               \
namespace oo{                                  \
    DefineCategory __##name##_Category(#name);      \
}

#define LOG_CATEGORY(name)                      \
    ((void*)(oo::__##name##_Category.pCategory))

//// in any.h
//DECLARE_LOG_CATEGORY(_Logic)
//
//// in any.cpp
//DEFINE_LOG_CATEGORY(_Logic)
//
//// direct log
//lLog(LOG_CATEGORY(_Logic), oo::LL_Info, "asdfasdf %d", num);
//oo::Log::Instance()->log("_Logic", oo::LL_Info, "asdfasdf %d", num);

//// and, can define macro
//#define Logic_Error(format, ...) lLog(LOG_CATEGORY(_Logic), oo::LL_Error, ##__VA_ARGS__);
//#define Logic_Warn(...) lLog(LOG_CATEGORY(_Logic), oo::LL_Warn, ##__VA_ARGS__);
//#define Logic_Info(...) lLog(LOG_CATEGORY(_Logic), oo::LL_Info, ##__VA_ARGS__);
//#define Logic_Debug(...) lLog(LOG_CATEGORY(_Logic), oo::LL_Debug, ##__VA_ARGS__);

//// then use
//UniG_Logic_Debug("asdfasdf %d", num);

#define lLog(category_void_ptr, level, format, ...) oo::Log::Instance()->log(category_void_ptr, level, format, ##_VA_ARGS__);

//// for old jade log(rLog), append to root category
#define lError(...) oo::Log::Instance()->log(oo::Log::mRootCategory, oo::LL_Error, ##__VA_ARGS__);
#define lWarning(...) oo::Log::Instance()->log(oo::Log::mRootCategory, oo::LL_Warn, ##__VA_ARGS__);
#define lInfo(...) oo::Log::Instance()->log(oo::Log::mRootCategory, oo::LL_Info, ##__VA_ARGS__);
#define lDebug(...) oo::Log::Instance()->log(oo::Log::mRootCategory, oo::LL_Debug, ##__VA_ARGS__);


//===================================================================================================
// stack trace
#define STACK_TRACE oo::StackRecord __SR__(__FUNCTION__)
#define STACK_STEP_BEGIN(name) oo::StackRecord::push(#name)
#define STACK_LOOP_BEGIN(name) oo::StackRecord::push(name)
#define STACK_STEP_END oo::StackRecord::pop()
