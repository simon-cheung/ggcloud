// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma warning(disable : 4003)
#pragma warning(disable : 4996)
#pragma warning(disable : 4267)

#include "osutils.h"
#include "logmacro.h"

// 预定义内部使用日志category
DECLARE_LOG_CATEGORY(NAF_Log);             

#define NAFError(format, ...) oo::Log::Instance()->log( __FILE__,__FUNCTION__,__LINE__, LOG_CATEGORY( NAF_Log ), oo::LL_Error, format, ##__VA_ARGS__)
#define NAFWarn(format,...) oo::Log::Instance()->log(  __FILE__,__FUNCTION__,__LINE__,LOG_CATEGORY( NAF_Log ), oo::LL_Warn, format, ##__VA_ARGS__)
#define NAFInfo(format,...) oo::Log::Instance()->log(  __FILE__,__FUNCTION__,__LINE__,LOG_CATEGORY( NAF_Log ), oo::LL_Info, format, ##__VA_ARGS__)
#define NAFDebug(format,...) oo::Log::Instance()->log(  __FILE__,__FUNCTION__,__LINE__,LOG_CATEGORY( NAF_Log ), oo::LL_Debug, format, ##__VA_ARGS__)


#include "asiosocket.h"
// TODO: reference additional headers your program requires here
