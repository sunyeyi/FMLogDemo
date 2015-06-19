#include "StdAfx.h"
#include "FMOutputDebugString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString FMFormatMessage(LPCTSTR lpszFormat, ...)
{
    va_list argList;
    va_start(argList, lpszFormat);

    CString strMsg;
    strMsg.FormatV(lpszFormat, argList);

    va_end(argList);

    return strMsg;
}
