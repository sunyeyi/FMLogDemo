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

    int x = 3;
    FMLOG_ASSERT((x == 2), _T("wrong expression"));
    FMLOG_ASSERT_V((x == 2), _T("wrong expression. x = %d"), x);
    x = 2;
    FMLOG_ASSERT((x == 2), _T("wrong expression"));
    FMLOG_ASSERT_V((x == 2), _T("wrong expression. x = %d"), x);
}

    FMLogEnd("The system exit normally.");

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

#define _FMLOG(Module, LogLevel, Message)            ((_FMLogNamespace g_FMLogger##Module.IsLevelLogged(LogLevel)) ? _FMLogNamespace g_FMLogger##Module.Write(Message, _T(#Module), LogLevel, __FILE__, __LINE__, __FUNCTION__) : 1)
#define _FMLOG_V(Module, LogLevel, pszFormat, ...)   ((_FMLogNamespace g_FMLogger##Module.IsLevelLogged(LogLevel)) ? _FMLogNamespace g_FMLogger##Module.Write(_FMLogNamespace FMFormatMessage(pszFormat, __VA_ARGS__), _T(#Module), LogLevel, __FILE__, __LINE__, __FUNCTION__) : 1)

////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface begin 
// 

//macros to define/declare a logger for special module
#define FMLOG_MODULE_DECLARE(Module)            _FMLogNamespaceBegin extern _FMLogNamespace CFMLogger  g_FMLogger##Module; _FMLogNamespaceEnd
#define FMLOG_MODULE_IMPLEMENT(Module)          _FMLogNamespaceBegin _FMLogNamespace CFMLogger  g_FMLogger##Module(_T(#Module)); _FMLogNamespaceEnd

//macros to log message 
#define FMLOG_INFO_SPE(Module, Message)            _FMLOG(Module,       _FMLogNamespace eFMLogLevelSpecialInfo, Message)
#define FMLOG_INFO_SPE_V(Module, pszFormat, ...)   _FMLOG_V(Module,     _FMLogNamespace eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)
#define FMLOG_INFO_SPE_N(Message)                  _FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelSpecialInfo, Message)
#define FMLOG_INFO_SPE_NV(pszFormat, ...)          _FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)

#define FMLOG_ERROR_UNK(Module, Message)           _FMLOG(Module,       _FMLogNamespace eFMLogLevelUnknown, Message)
#define FMLOG_ERROR_UNK_V(Module, pszFormat, ...)  _FMLOG_V(Module,     _FMLogNamespace eFMLogLevelUnknown, pszFormat, __VA_ARGS__)
#define FMLOG_ERROR_UNK_N(Message)                 _FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelUnknown, Message)
#define FMLOG_ERROR_UNK_NV(pszFormat, ...)         _FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelUnknown, pszFormat, __VA_ARGS__)

#define FMLOG_FATAL(Module, Message)               _FMLOG(Module,       _FMLogNamespace eFMLogLevelFatal, Message)
#define FMLOG_FATAL_V(Module, pszFormat, ...)      _FMLOG_V(Module,     _FMLogNamespace eFMLogLevelFatal, pszFormat, __VA_ARGS__)
#define FMLOG_FATAL_N(Message)                     _FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelFatal, Message)
#define FMLOG_FATAL_NV(pszFormat, ...)             _FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelFatal, pszFormat, __VA_ARGS__)

#define FMLOG_ERROR(Module, Message)               _FMLOG(Module,       _FMLogNamespace eFMLogLevelError, Message)
#define FMLOG_ERROR_V(Module, pszFormat, ...)      _FMLOG_V(Module,     _FMLogNamespace eFMLogLevelError, pszFormat, __VA_ARGS__)
#define FMLOG_ERROR_N(Message)                     _FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelError, Message)
#define FMLOG_ERROR_NV(pszFormat, ...)             _FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelError, pszFormat, __VA_ARGS__)

#define FMLOG_WARN(Module, Message)                _FMLOG(Module,       _FMLogNamespace eFMLogLevelWarning, Message)
#define FMLOG_WARN_V(Module, pszFormat, ...)       _FMLOG_V(Module,     _FMLogNamespace eFMLogLevelWarning, pszFormat, __VA_ARGS__)
#define FMLOG_WARN_N(Message)                      _FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelWarning, Message)
#define FMLOG_WARN_NV(pszFormat, ...)              _FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelWarning, pszFormat, __VA_ARGS__)

#define FMLOG_INFO(Module, Message)                _FMLOG(Module,       _FMLogNamespace eFMLogLevelInfo, Message)
#define FMLOG_INFO_V(Module, pszFormat, ...)       _FMLOG_V(Module,     _FMLogNamespace eFMLogLevelInfo, pszFormat, __VA_ARGS__)
#define FMLOG_INFO_N(Message)                      _FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelInfo, Message)
#define FMLOG_INFO_NV(pszFormat, ...)              _FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelInfo, pszFormat, __VA_ARGS__)

#define FMLOG_DEBUG(Module, Message)               _FMLOG(Module,       _FMLogNamespace eFMLogLevelDebug, Message)
#define FMLOG_DEBUG_V(Module, pszFormat, ...)      _FMLOG_V(Module,     _FMLogNamespace eFMLogLevelDebug, pszFormat, __VA_ARGS__)
#define FMLOG_DEBUG_N(Message)                     _FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelDebug, Message)
#define FMLOG_DEBUG_NV(pszFormat, ...)             _FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelDebug, pszFormat, __VA_ARGS__)

#define FMLOG_TRACE(Module, Message)               _FMLOG(Module,       _FMLogNamespace eFMLogLevelTrace, Message)
#define FMLOG_TRACE_V(Module, pszFormat, ...)      _FMLOG_V(Module,     _FMLogNamespace eFMLogLevelTrace, pszFormat, __VA_ARGS__)
#define FMLOG_TRACE_N(Message)                     _FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelTrace, Message)
#define FMLOG_TRACE_NV(pszFormat, ...)             _FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelTrace, pszFormat, __VA_ARGS__)

#define FMLOG_TEMP(Module, Message)                _FMLOG(Module,       _FMLogNamespace eFMLogLevelTemp, Message)
#define FMLOG_TEMP_V(Module, pszFormat, ...)       _FMLOG_V(Module,     _FMLogNamespace eFMLogLevelTemp, pszFormat, __VA_ARGS__)
#define FMLOG_TEMP_N(Message)                      _FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelTemp, Message)
#define FMLOG_TEMP_NV(pszFormat, ...)              _FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelTemp, pszFormat, __VA_ARGS__)

#define FMLOG_ALWAYS(Module, Message)              _FMLOG(Module,       _FMLogNamespace eFMLogLevelSpecialInfo, Message)
#define FMLOG_ALWAYS_V(Module, pszFormat, ...)     _FMLOG_V(Module,     _FMLogNamespace eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)
#define FMLOG_ALWAYS_N(Message)                    _FMLOG(FMGlobal,     _FMLogNamespace eFMLogLevelSpecialInfo, Message)
#define FMLOG_ALWAYS_NV(pszFormat, ...)            _FMLOG_V(FMGlobal,   _FMLogNamespace eFMLogLevelSpecialInfo, pszFormat, __VA_ARGS__)

#define FMLOG_ASSERT(Expression, Message)          ((!(Expression)) ? _FMLOG(FMAssert,     _FMLogNamespace eFMLogLevelFatal, CString(_T(#Expression)_T(", ")) + Message) : 1)
#define FMLOG_ASSERT_V(Expression, pszFormat, ...) ((!(Expression)) ? _FMLOG_V(FMAssert,   _FMLogNamespace eFMLogLevelFatal, CString(_T(#Expression)_T(", ")) + pszFormat, __VA_ARGS__) : 1)

#define FMLOG_END(Message)                         FMLogEnd(Message, __FILE__, __LINE__, __FUNCTION__);

void SetLogLevel(EFMLogLevel eLogLevel);

// 
// Interface end 
////////////////////////////////////////////////////////////////////////////////////////////////////

void FMLogEnd(const CString& strMessage, LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction);

_FMLogNamespaceEnd


//the Global instance to log global message which are not belong to any module.
FMLOG_MODULE_DECLARE(FMGlobal);
FMLOG_MODULE_DECLARE(FMAssert);


#endif // FMLoggerInterface_h__

///@}//end of addtogroup FMLog FMLog
