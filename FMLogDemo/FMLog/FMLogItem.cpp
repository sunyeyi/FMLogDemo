///@file FMLogItem.cpp
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
#include "FMLogItem.h"
#include "FMLogNamespaceMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

_FMLogNamespaceBegin

void CFMLogItem::SetLogMsg(const CString& strErrMsg, const CString strUser, const CString& strModule, const CString& strLevel
    , LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction
    )
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    DWORD nProcessID = ::GetCurrentProcessId();
    DWORD nThreadID = ::GetCurrentThreadId();

    if (strErrMsg.IsEmpty())
    {
        m_strLogMsg.Format(_T("[%s] %04d%02d%02d_%02d:%02d:%02d.%03d <%X:%X> %s %s %s(%d),%s\n"),
            strLevel, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
            , nProcessID, nThreadID, strUser, strModule
            , CString(strSourceCodeFile), nSourceCodeLine, CString(strSourceCodeFunction)
            );
    }
    else
    {
        m_strLogMsg.Format(_T("[%s] %04d%02d%02d_%02d:%02d:%02d.%03d <%X:%X> %s %s %s(%d),%s,%s\n"),
            strLevel, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
            , nProcessID, nThreadID, strUser, strModule
            , CString(strSourceCodeFile), nSourceCodeLine, CString(strSourceCodeFunction)
            , strErrMsg
            );
    }
}

void CFMLogItem::SetLogMsg(const CString& val)
{
    m_strLogMsg = val;
}

const CString& CFMLogItem::GetLogMsg() const
{
    return m_strLogMsg;
}

_FMLogNamespaceEnd

