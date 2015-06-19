
#ifndef FMLoggerInterface_h__
#define FMLoggerInterface_h__

////////////////////////////////////////////////////////////////////////////////////////////////////
// Usage: 
/*

FMLOG_MODULE_DECLARE(TestModule);
FMLOG_MODULE_IMPLEMENT(TestModule);

void MoreAndMoreTest()
{
    FMLOG_INFO_SPE(TestModule, _T("log message "));
    FMLOG_INFO_SPE_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_INFO_SPE_N(_T("log message "));
    FMLOG_INFO_SPE_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_ERROR_UNK(TestModule, _T("log message "));
    FMLOG_ERROR_UNK_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_ERROR_UNK_N(_T("log message "));
    FMLOG_ERROR_UNK_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_FATAL(TestModule, _T("log message "));
    FMLOG_FATAL_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_FATAL_N(_T("log message "));
    FMLOG_FATAL_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_ERROR(TestModule, _T("log message "));
    FMLOG_ERROR_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_ERROR_N(_T("log message "));
    FMLOG_ERROR_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_WARN(TestModule, _T("log message "));
    FMLOG_WARN_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_WARN_N(_T("log message "));
    FMLOG_WARN_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_INFO(TestModule, _T("log message "));
    FMLOG_INFO_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_INFO_N(_T("log message "));
    FMLOG_INFO_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_DEBUG(TestModule, _T("log message "));
    FMLOG_DEBUG_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_DEBUG_N(_T("log message "));
    FMLOG_DEBUG_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_TRACE(TestModule, _T("log message "));
    FMLOG_TRACE_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_TRACE_N(_T("log message "));
    FMLOG_TRACE_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_TEMP(TestModule, _T("log message "));
    FMLOG_TEMP_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_TEMP_N(_T("log message "));
    FMLOG_TEMP_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));

    FMLOG_ALWAYS(TestModule, _T("log message "));
    FMLOG_ALWAYS_V(TestModule, _T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
    FMLOG_ALWAYS_N(_T("log message "));
    FMLOG_ALWAYS_NV(_T("log message %d,%f,%s"), 18, 123.4, _T("Hello FMLog"));
}

//
// How to debug FMLog itself?
// 1.
#define DEBUG_LOG_WRITER 1
// 2.
FMOutputDebugString & DebugView

*/


#include "FMLogger.h"
#include "FMFormatMessage.h"

#pragma once

#define FMLOG(Module, LogLevel, Message)            ((g_FMLogger##Module.IsLevelLogged(LogLevel)) ? g_FMLogger##Module.Write(Message, _T(#Module), LogLevel, __FILE__, __LINE__, __FUNCTION__) : 1)
#define FMLOG_V(Module, LogLevel, pszFormat, ...)   ((g_FMLogger##Module.IsLevelLogged(LogLevel)) ? g_FMLogger##Module.Write(FMFormatMessage(pszFormat, __VA_ARGS__), _T(#Module), LogLevel, __FILE__, __LINE__, __FUNCTION__) : 1)

////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface begin 
// 

//macros to define/declare a logger for special module
#define FMLOG_MODULE_DECLARE(Module)            extern CFMLogger g_FMLogger##Module
#define FMLOG_MODULE_IMPLEMENT(Module)          CFMLogger g_FMLogger##Module(_T(#Module))

//macros to log message 
#define FMLOG_INFO_SPE(Module, Message)            FMLOG(Module,       eFMLogLevelSpecialInfo, Message)
#define FMLOG_INFO_SPE_V(Module, pszFormat, ...)   FMLOG_V(Module,     eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)
#define FMLOG_INFO_SPE_N(Message)                  FMLOG(FMGlobal,     eFMLogLevelSpecialInfo, Message)
#define FMLOG_INFO_SPE_NV(pszFormat, ...)          FMLOG_V(FMGlobal,   eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)

#define FMLOG_ERROR_UNK(Module, Message)           FMLOG(Module,       eFMLogLevelUnknown, Message)
#define FMLOG_ERROR_UNK_V(Module, pszFormat, ...)  FMLOG_V(Module,     eFMLogLevelUnknown, pszFormat, __VA_ARGS__)
#define FMLOG_ERROR_UNK_N(Message)                 FMLOG(FMGlobal,     eFMLogLevelUnknown, Message)
#define FMLOG_ERROR_UNK_NV(pszFormat, ...)         FMLOG_V(FMGlobal,   eFMLogLevelUnknown, pszFormat, __VA_ARGS__)

#define FMLOG_FATAL(Module, Message)               FMLOG(Module,       eFMLogLevelFatal, Message)
#define FMLOG_FATAL_V(Module, pszFormat, ...)      FMLOG_V(Module,     eFMLogLevelFatal, pszFormat, __VA_ARGS__)
#define FMLOG_FATAL_N(Message)                     FMLOG(FMGlobal,     eFMLogLevelFatal, Message)
#define FMLOG_FATAL_NV(pszFormat, ...)             FMLOG_V(FMGlobal,   eFMLogLevelFatal, pszFormat, __VA_ARGS__)

#define FMLOG_ERROR(Module, Message)               FMLOG(Module,       eFMLogLevelError, Message)
#define FMLOG_ERROR_V(Module, pszFormat, ...)      FMLOG_V(Module,     eFMLogLevelError, pszFormat, __VA_ARGS__)
#define FMLOG_ERROR_N(Message)                     FMLOG(FMGlobal,     eFMLogLevelError, Message)
#define FMLOG_ERROR_NV(pszFormat, ...)             FMLOG_V(FMGlobal,   eFMLogLevelError, pszFormat, __VA_ARGS__)

#define FMLOG_WARN(Module, Message)                FMLOG(Module,       eFMLogLevelWarning, Message)
#define FMLOG_WARN_V(Module, pszFormat, ...)       FMLOG_V(Module,     eFMLogLevelWarning, pszFormat, __VA_ARGS__)
#define FMLOG_WARN_N(Message)                      FMLOG(FMGlobal,     eFMLogLevelWarning, Message)
#define FMLOG_WARN_NV(pszFormat, ...)              FMLOG_V(FMGlobal,   eFMLogLevelWarning, pszFormat, __VA_ARGS__)

#define FMLOG_INFO(Module, Message)                FMLOG(Module,       eFMLogLevelInfo, Message)
#define FMLOG_INFO_V(Module, pszFormat, ...)       FMLOG_V(Module,     eFMLogLevelInfo, pszFormat, __VA_ARGS__)
#define FMLOG_INFO_N(Message)                      FMLOG(FMGlobal,     eFMLogLevelInfo, Message)
#define FMLOG_INFO_NV(pszFormat, ...)              FMLOG_V(FMGlobal,   eFMLogLevelInfo, pszFormat, __VA_ARGS__)

#define FMLOG_DEBUG(Module, Message)               FMLOG(Module,       eFMLogLevelDebug, Message)
#define FMLOG_DEBUG_V(Module, pszFormat, ...)      FMLOG_V(Module,     eFMLogLevelDebug, pszFormat, __VA_ARGS__)
#define FMLOG_DEBUG_N(Message)                     FMLOG(FMGlobal,     eFMLogLevelDebug, Message)
#define FMLOG_DEBUG_NV(pszFormat, ...)             FMLOG_V(FMGlobal,   eFMLogLevelDebug, pszFormat, __VA_ARGS__)

#define FMLOG_TRACE(Module, Message)               FMLOG(Module,       eFMLogLevelTrace, Message)
#define FMLOG_TRACE_V(Module, pszFormat, ...)      FMLOG_V(Module,     eFMLogLevelTrace, pszFormat, __VA_ARGS__)
#define FMLOG_TRACE_N(Message)                     FMLOG(FMGlobal,     eFMLogLevelTrace, Message)
#define FMLOG_TRACE_NV(pszFormat, ...)             FMLOG_V(FMGlobal,   eFMLogLevelTrace, pszFormat, __VA_ARGS__)

#define FMLOG_TEMP(Module, Message)                FMLOG(Module,       eFMLogLevelTemp, Message)
#define FMLOG_TEMP_V(Module, pszFormat, ...)       FMLOG_V(Module,     eFMLogLevelTemp, pszFormat, __VA_ARGS__)
#define FMLOG_TEMP_N(Message)                      FMLOG(FMGlobal,     eFMLogLevelTemp, Message)
#define FMLOG_TEMP_NV(pszFormat, ...)              FMLOG_V(FMGlobal,   eFMLogLevelTemp, pszFormat, __VA_ARGS__)

#define FMLOG_ALWAYS(Module, Message)              FMLOG(Module,       eFMLogLevelSpecialInfo, Message)
#define FMLOG_ALWAYS_V(Module, pszFormat, ...)     FMLOG_V(Module,     eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)
#define FMLOG_ALWAYS_N(Message)                    FMLOG(FMGlobal,     eFMLogLevelSpecialInfo, Message)
#define FMLOG_ALWAYS_NV(pszFormat, ...)            FMLOG_V(FMGlobal,   eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)


void SetLogLevel(EFMLogLevel eLogLevel);

// 
// Interface end 
////////////////////////////////////////////////////////////////////////////////////////////////////


//the Global instance to log global message which are not belong to any module.
FMLOG_MODULE_DECLARE(FMGlobal);


#endif // FMLoggerInterface_h__
