#ifndef FMLogItem_h__
#define FMLogItem_h__

#pragma once

#include <queue>
#include <afxmt.h>

class CFMLogItem
{
public:
    void SetLogMsg(const CString&);
    void SetLogMsg(const CString& strErrMsg, const CString strUser, const CString& strModule, const CString& strLevel
        , LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction
        );
    const CString& GetLogMsg() const;

private:
    CString m_strLogMsg;
};

#endif // FMLogItem_h__