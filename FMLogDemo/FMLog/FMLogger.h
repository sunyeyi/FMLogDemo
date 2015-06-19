
#ifndef FMLogger_h__
#define FMLogger_h__

#pragma once

#include <map>
#include "FMLogLevel.h"
#include "FMLogNamespaceMacro.h"

_FMLogNamespaceBegin


class CFMLogWriter;

class CFMLogger
{
public:
    CFMLogger(const CString& strModule);
    ~CFMLogger();

public:
    void SetLogLevel(EFMLogLevel eLogLevel);
    bool IsLevelLogged(EFMLogLevel eMessageLevel);
    void Write(const CString& strErrMsg, const CString strUser, EFMLogLevel eMessageLevel
        , LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction
        );

private:
    CString m_strModule;
    EFMLogLevel m_eLogLevel;

private:
    static CFMLogWriter& ms_refLogWriter;
};


_FMLogNamespaceEnd

#endif // FMLogger_h__

