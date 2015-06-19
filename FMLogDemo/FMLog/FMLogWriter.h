
#ifndef FMLogWriter_h__
#define FMLogWriter_h__

#pragma once

#include <queue>
#include <afxmt.h>
#include "FMUncopyable.h"
#include "FMThreadSafeQueue.h"
#include "FMLogNamespaceMacro.h"

_FMLogNamespaceBegin

class CFMLogItem;

class CFMLogWriter : private CFMUncopyable
{
public:
    static CFMLogWriter& GetInstance();
    ~CFMLogWriter();

private:
    CFMLogWriter();

public:
    void SetLogFileFolder(const CString& strLogFileFolder);
    void SetLogFileMaxSize(size_t nMaxSizeMegabyte); //unit: M

public:
    void Write(const CString& strErrMsg, const CString strUser, const CString& strModule, const CString& strLevel
        , LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction
        );

public:
    static UINT ThreadProc(LPVOID lpParam);
    UINT WriteThreadProc();
    CEvent m_WriteEvent;
    CWinThread* m_pWriteThread;
    volatile bool m_bExitWriteThread;

private:
    CFMThreadSafeLogQueue m_logQueue;
    void AddLogToQueue(const CString& strErrMsg, const CString strUser, const CString& strModule, const CString& strLevel
        , LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction
        );

private:
    void WriteLogOfLogThread(bool bStart);
    void SetLogFileName(CString val); //full path

private:
    CString GenerateBackupLogFileName();
    int WriteToFile(CFMLogItem* pLogItem);
    int WriteToFileInternal(CFMLogItem* pLogItem);
    void WriteToFileInternal(CFMLogItem* pLogItem, CStdioFile& oFile);

private:
    CString m_strLogFileFolder;
    CString m_strLogFileName; //full path
    size_t m_nMaxSizeByteOfLogFile;
    CMutex*  m_pLogFileMutex;
};

_FMLogNamespaceEnd

#endif // FMLogWriter_h__

