#include "stdafx.h"
#include "FMLogger.h"
#include "FMLogWriter.h"
#include "FMLoggerManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFMLogWriter& CFMLogger::ms_refLogWriter = CFMLogWriter::GetInstance();

CFMLogger::CFMLogger(const CString& strModule)
    : m_strModule(strModule)
{
    m_eLogLevel = eFMLogLevelInfo;

    CFMLoggerManager::GetInstance().Add(strModule, this);
}

CFMLogger::~CFMLogger()
{
}

bool CFMLogger::IsLevelLogged(EFMLogLevel eMessageLevel)
{
    if (eMessageLevel <= m_eLogLevel)
    {
        return true;
    }

    return false;
}

void CFMLogger::Write(const CString& strErrMsg, const CString strUser, EFMLogLevel eMessageLevel, LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction)
{
    ms_refLogWriter.Write(strErrMsg, strUser, m_strModule, CFMLogLevel::To3LetterString(eMessageLevel)
        , strSourceCodeFile, nSourceCodeLine, strSourceCodeFunction);
}

void CFMLogger::SetLogLevel(EFMLogLevel eLogLevel)
{
    m_eLogLevel = eLogLevel;
}