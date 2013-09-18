// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <osutils.h>
#include <logmacro.h>
#include <bigint.h>
#include <asiosocket.h>

// 预定义内部使用日志category
DECLARE_LOG_CATEGORY(KD_Log);             

#define KDError(format, ...) oo::Log::Instance()->log( __FILE__,__FUNCTION__,__LINE__, LOG_CATEGORY( KD_Log ), oo::LL_Error, format, ##__VA_ARGS__)
#define KDWarn(format,...) oo::Log::Instance()->log(  __FILE__,__FUNCTION__,__LINE__,LOG_CATEGORY( KD_Log ), oo::LL_Warn, format, ##__VA_ARGS__)
#define KDInfo(format,...) oo::Log::Instance()->log(  __FILE__,__FUNCTION__,__LINE__,LOG_CATEGORY( KD_Log ), oo::LL_Info, format, ##__VA_ARGS__)
#define KDDebug(format,...) oo::Log::Instance()->log(  __FILE__,__FUNCTION__,__LINE__,LOG_CATEGORY( KD_Log ), oo::LL_Debug, format, ##__VA_ARGS__)

// TODO: reference additional headers your program requires here
