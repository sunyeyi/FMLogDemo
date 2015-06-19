///@file FMLoggerInterface.h
/// <summary>
///   
/// </summary>
/// -------------------------------------------------------------------------<BR>
/// <description>
/// <BR>
/// </description>
/// <BR>
/// Created By: yysun <BR>
/// Creation Date: Friday June 19, 2015 <BR> 
/// Reviewed By: <BR>
/// Review Date: <BR>
/// -------------------------------------------------------------------------<BR>
///@addtogroup FMLog FMLog
///@{

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
#include "FMLogNamespaceMacro.h"

#pragma once

_FMLogNamespaceBegin

#define FMLOG(Module, LogLevel, Message)            ((_FMLogNamespace g_FMLogger##Module.IsLevelLogged(LogLevel)) ? _FMLogNamespace g_FMLogger##Module.Write(Message, _T(#Module), LogLevel, __FILE__, __LINE__, __FUNCTION__) : 1)
#define FMLOG_V(Module, LogLevel, pszFormat, ...)   ((_FMLogNamespace g_FMLogger##Module.IsLevelLogged(LogLevel)) ? _FMLogNamespace g_FMLogger##Module.Write(_FMLogNamespace FMFormatMessage(pszFormat, __VA_ARGS__), _T(#Module), LogLevel, __FILE__, __LINE__, __FUNCTION__) : 1)

////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface begin 
// 

//macros to define/declare a logger for special module
#define FMLOG_MODULE_DECLARE(Module)            _FMLogNamespaceBegin extern _FMLogNamespace CFMLogger  g_FMLogger##Module; _FMLogNamespaceEnd
#define FMLOG_MODULE_IMPLEMENT(Module)          _FMLogNamespaceBegin _FMLogNamespace CFMLogger  g_FMLogger##Module(_T(#Module)); _FMLogNamespaceEnd

//macros to log message 
#define FMLOG_INFO_SPE(Module, Message)            FMLOG(Module,       _FMLogNamespace eFMLogLevelSpecialInfo, Message)
#define FMLOG_INFO_SPE_V(Module, pszFormat, ...)   FMLOG_V(Module,     _FMLogNamespace eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)
#define FMLOG_INFO_SPE_N(Message)                  FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelSpecialInfo, Message)
#define FMLOG_INFO_SPE_NV(pszFormat, ...)          FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)

#define FMLOG_ERROR_UNK(Module, Message)           FMLOG(Module,       _FMLogNamespace eFMLogLevelUnknown, Message)
#define FMLOG_ERROR_UNK_V(Module, pszFormat, ...)  FMLOG_V(Module,     _FMLogNamespace eFMLogLevelUnknown, pszFormat, __VA_ARGS__)
#define FMLOG_ERROR_UNK_N(Message)                 FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelUnknown, Message)
#define FMLOG_ERROR_UNK_NV(pszFormat, ...)         FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelUnknown, pszFormat, __VA_ARGS__)

#define FMLOG_FATAL(Module, Message)               FMLOG(Module,       _FMLogNamespace eFMLogLevelFatal, Message)
#define FMLOG_FATAL_V(Module, pszFormat, ...)      FMLOG_V(Module,     _FMLogNamespace eFMLogLevelFatal, pszFormat, __VA_ARGS__)
#define FMLOG_FATAL_N(Message)                     FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelFatal, Message)
#define FMLOG_FATAL_NV(pszFormat, ...)             FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelFatal, pszFormat, __VA_ARGS__)

#define FMLOG_ERROR(Module, Message)               FMLOG(Module,       _FMLogNamespace eFMLogLevelError, Message)
#define FMLOG_ERROR_V(Module, pszFormat, ...)      FMLOG_V(Module,     _FMLogNamespace eFMLogLevelError, pszFormat, __VA_ARGS__)
#define FMLOG_ERROR_N(Message)                     FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelError, Message)
#define FMLOG_ERROR_NV(pszFormat, ...)             FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelError, pszFormat, __VA_ARGS__)

#define FMLOG_WARN(Module, Message)                FMLOG(Module,       _FMLogNamespace eFMLogLevelWarning, Message)
#define FMLOG_WARN_V(Module, pszFormat, ...)       FMLOG_V(Module,     _FMLogNamespace eFMLogLevelWarning, pszFormat, __VA_ARGS__)
#define FMLOG_WARN_N(Message)                      FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelWarning, Message)
#define FMLOG_WARN_NV(pszFormat, ...)              FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelWarning, pszFormat, __VA_ARGS__)

#define FMLOG_INFO(Module, Message)                FMLOG(Module,       _FMLogNamespace eFMLogLevelInfo, Message)
#define FMLOG_INFO_V(Module, pszFormat, ...)       FMLOG_V(Module,     _FMLogNamespace eFMLogLevelInfo, pszFormat, __VA_ARGS__)
#define FMLOG_INFO_N(Message)                      FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelInfo, Message)
#define FMLOG_INFO_NV(pszFormat, ...)              FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelInfo, pszFormat, __VA_ARGS__)

#define FMLOG_DEBUG(Module, Message)               FMLOG(Module,       _FMLogNamespace eFMLogLevelDebug, Message)
#define FMLOG_DEBUG_V(Module, pszFormat, ...)      FMLOG_V(Module,     _FMLogNamespace eFMLogLevelDebug, pszFormat, __VA_ARGS__)
#define FMLOG_DEBUG_N(Message)                     FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelDebug, Message)
#define FMLOG_DEBUG_NV(pszFormat, ...)             FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelDebug, pszFormat, __VA_ARGS__)

#define FMLOG_TRACE(Module, Message)               FMLOG(Module,       _FMLogNamespace eFMLogLevelTrace, Message)
#define FMLOG_TRACE_V(Module, pszFormat, ...)      FMLOG_V(Module,     _FMLogNamespace eFMLogLevelTrace, pszFormat, __VA_ARGS__)
#define FMLOG_TRACE_N(Message)                     FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelTrace, Message)
#define FMLOG_TRACE_NV(pszFormat, ...)             FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelTrace, pszFormat, __VA_ARGS__)

#define FMLOG_TEMP(Module, Message)                FMLOG(Module,       _FMLogNamespace eFMLogLevelTemp, Message)
#define FMLOG_TEMP_V(Module, pszFormat, ...)       FMLOG_V(Module,     _FMLogNamespace eFMLogLevelTemp, pszFormat, __VA_ARGS__)
#define FMLOG_TEMP_N(Message)                      FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelTemp, Message)
#define FMLOG_TEMP_NV(pszFormat, ...)              FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelTemp, pszFormat, __VA_ARGS__)

#define FMLOG_ALWAYS(Module, Message)              FMLOG(Module,       _FMLogNamespace eFMLogLevelSpecialInfo, Message)
#define FMLOG_ALWAYS_V(Module, pszFormat, ...)     FMLOG_V(Module,     _FMLogNamespace eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)
#define FMLOG_ALWAYS_N(Message)                    FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelSpecialInfo, Message)
#define FMLOG_ALWAYS_NV(pszFormat, ...)            FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)


void SetLogLevel(EFMLogLevel eLogLevel);

// 
// Interface end 
////////////////////////////////////////////////////////////////////////////////////////////////////

_FMLogNamespaceEnd


//the Global instance to log global message which are not belong to any module.
FMLOG_MODULE_DECLARE(FMGlobal);


#endif // FMLoggerInterface_h__

///@}//end of addtogroup FMLog FMLog
