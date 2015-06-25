///@file FMLogWriter.h
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

#ifndef FMLogWriter_h__
#define FMLogWriter_h__

#pragma once

#include <queue>
#include <afxmt.h>
#include "FMUncopyable.h"
#include "FMThreadSafeQueue.h"
#include "FMLogNamespaceMacro.h"

_FMLogNamespaceBegin

enum  EFMLogWriterThreadState
{
    eLogThreadIsNotStart,
    eLogThreadIsRunning,
    eLogThreadIsFinished,
    eLogThreadIsFinishedWithUnknowError,
};

class CFMLogItem;

class CFMLogWriter : private CFMUncopyable
{
public:
    static CFMLogWriter& GetInstance();
    ~CFMLogWriter();
    
public:
    void EndWriterThread();

private:
    CFMLogWriter();
    void BeginWriterThread();

public:
    void SetLogFileFolder(const CString& strLogFileFolder);
    void SetLogFileMaxSize(size_t nMaxSizeMegabyte); //unit: M

public:
    // 
    // ONLY this function can write log message. You should NOT call any other function to write log.
    // 
    void Write(const CString& strErrMsg, const CString strUser, const CString& strModule, const CString& strLevel
        , LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction
        );

public:
    // Only for FMLog inner
    void WriteLogOfLogWriter(const CString& strMsg, LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction);

public:
    static UINT ThreadProc(LPVOID lpParam);
    UINT WriteThreadProc();
    CEvent m_WriteEvent;
    CWinThread* m_pWriteThread;
    volatile bool m_bExitWriteThread;
    EFMLogWriterThreadState m_logThreadState;
    bool IsLogWriterThreadFinished();

private:
    CFMThreadSafeLogQueue m_logQueue;
    void AddLogToQueue(const CString& strErrMsg, const CString strUser, const CString& strModule, const CString& strLevel
        , LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction
        );

private:
    void SetLogFileName(CString val); //full path

private:
    CString GenerateBackupLogFileName();
    int WriteToFile(CFMLogItem* pLogItem);
    UINT WriteToFileInternal(CFMLogItem* pLogItem);
    void WriteToFileInternal(CFMLogItem* pLogItem, CStdioFile& oFile);

private:
    CString m_strLogFileFolder;
    CString m_strLogFileName; //full path
    size_t m_nMaxSizeByteOfLogFile;
    CMutex*  m_pLogFileMutex;
};

_FMLogNamespaceEnd

#endif // FMLogWriter_h__


///@}//end of addtogroup FMLog FMLog
