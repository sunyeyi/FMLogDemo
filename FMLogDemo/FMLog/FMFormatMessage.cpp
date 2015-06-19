///@file FMFormatMessage.cpp
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

#include "StdAfx.h"
#include "FMOutputDebugString.h"
#include "FMLogNamespaceMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

_FMLogNamespaceBegin

CString FMFormatMessage(LPCTSTR lpszFormat, ...)
{
    va_list argList;
    va_start(argList, lpszFormat);

    CString strMsg;
    strMsg.FormatV(lpszFormat, argList);

    va_end(argList);

    return strMsg;
}

_FMLogNamespaceEnd