///@file FMOutputDebugString.cpp
/// <summary>
///   
/// </summary>
/// -------------------------------------------------------------------------<BR>
/// <description>
/// <BR>
/// </description>
/// <BR>
/// Created By: yysun<BR>
/// Creation Date: Friday June 19, 2015 <BR>  
/// Reviewed By: <BR>
/// Review Date: <BR>
/// -------------------------------------------------------------------------<BR>

#include "StdAfxFMDLL.h"
#include "FMOutputDebugString.h"
#include "FMLogNamespaceMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

_FMLogNamespaceBegin

const CString g_strLogModuleName = _T("FMLogSys");
static const CString g_strFM = _T("#FM# ");
static const CString g_strFMLog = _T("#FM# FMLogSys ");

void FMOutputDebugString(LPCTSTR lpszFormat, ...)
{
    va_list argList;
    va_start(argList, lpszFormat);
    
    CString strMsg;
    strMsg.FormatV(lpszFormat, argList);

    va_end(argList);

    OutputDebugString(g_strFM + strMsg);
}

void FMLogOutputDebugString(LPCTSTR lpszFormat, ...)
{
    va_list argList;
    va_start(argList, lpszFormat);

    CString strMsg;
    strMsg.FormatV(lpszFormat, argList);

    va_end(argList);

    OutputDebugString(g_strFMLog + strMsg);
}

_FMLogNamespaceEnd