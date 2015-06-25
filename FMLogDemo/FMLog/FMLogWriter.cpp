///@file FMLogWriter.cpp
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
#include "FMLogWriter.h"
#include "FMLogItem.h"
#include "FMLogLevel.h"
#include "FMOutputDebugString.h"
#include "FMLogNamespaceMacro.h"
#include "FMSetThreadName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern void FMLogOutputDebugString(LPCTSTR lpszFormat, ...);

#define DEBUG_LOG_WRITER 1

#define FMLogLog(Message) this->WriteLogOfLogWriter(Message, __FILE__, __LINE__, __FUNCTION__);

_FMLogNamespaceBegin

//default log file size: 20M 
#define MaxSizeMegabyteOfLogFile 100
#define MaxLogQueueSize 10000

enum EFMLogWriterThreadExitCode
{
    eLogWriterExitCode_OK = 0,

    eLogWriterExitCode_ThreadFunctionUnkownError,
    eLogWriterExitCode_ThreadFunctionMemoryException,
    eLogWriterExitCode_ThreadFunctionFileException,
    eLogWriterExitCode_ThreadFunctionException,
    eLogWriterExitCode_ThreadFunctionUnknownException,

    eLogWriterExitCode_WriteToFileUnkownError,
    eLogWriterExitCode_WriteToFileMemoryException,
    eLogWriterExitCode_WriteToFileFileException,
    eLogWriterExitCode_WriteToFileException,
    eLogWriterExitCode_WriteToFileUnknownException,
    
    eLogWriterExitCode_OpenLogFileFailed,
    eLogWriterExitCode_OpenLogFileFailed_SharingViolation,
};

static LPCTSTR szLogFileName = _T("FMLog.txt");

CFMLogWriter::CFMLogWriter()
    : m_bExitWriteThread(false)
    , m_logQueue(MaxLogQueueSize)
    , m_pLogFileMutex(nullptr)
    , m_pWriteThread(nullptr)
    , m_logThreadState(eLogThreadIsNotStart)
{
    SetLogFileName(szLogFileName);

    BeginWriterThread();
}

CFMLogWriter::~CFMLogWriter()
{
    EndWriterThread();
}

void CFMLogWriter::SetLogFileFolder(const CString& strLogFileFolder)
{
    m_strLogFileFolder = strLogFileFolder;
    SetLogFileName(m_strLogFileFolder + _T("\\") + szLogFileName);
}

void CFMLogWriter::Write(const CString& strErrMsg, const CString strUser, const CString& strModule, const CString& strLevel
    , LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction
    )
{
    if (IsLogWriterThreadFinished())
    {
        // log writer thread is available.
        AddLogToQueue(strErrMsg, strUser, strModule, strLevel
            , strSourceCodeFile, nSourceCodeLine, strSourceCodeFunction);
    }
    else
    {
        CFMLogItem oLogItem;
        oLogItem.SetLogMsg(strErrMsg, strUser, strModule, strLevel
            , strSourceCodeFile, nSourceCodeLine, strSourceCodeFunction);
        FMLogOutputDebugString(oLogItem.GetLogMsg());
    }
}

CString CFMLogWriter::GenerateBackupLogFileName()
{
    CString strBackupLogFileName;

    do
    {
        SYSTEMTIME st;
        GetLocalTime(&st);

        strBackupLogFileName = m_strLogFileName;

        ASSERT(strBackupLogFileName[strBackupLogFileName.GetLength() - 4] == '.');
        strBackupLogFileName.Format(_T("%s_%04d%02d%02d_%02d%02d%02d%03d.%s")
            , strBackupLogFileName.Left(strBackupLogFileName.GetLength() - 4)
            , st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
            , strBackupLogFileName.Right(3)
            );
    } 
    while (PathFileExists(strBackupLogFileName));

    return strBackupLogFileName;
}

CFMLogWriter& CFMLogWriter::GetInstance()
{
    static CFMLogWriter theLogWriter;
    return theLogWriter;
}

UINT CFMLogWriter::ThreadProc(LPVOID lpParam)
{
    FMLogOutputDebugString(_T("Log writer thread process start.\n"));

    UINT nRet = eLogWriterExitCode_ThreadFunctionUnkownError;

    try
    {
        CFMLogWriter* pLogWriter = (CFMLogWriter*)lpParam;
        ASSERT(pLogWriter != nullptr);
        nRet = pLogWriter->WriteThreadProc();
    }
    catch (CMemoryException* e)
    {
        nRet = eLogWriterExitCode_ThreadFunctionMemoryException;
        ASSERT(FALSE);
        e->ReportError();

        TCHAR szErrorMessage[512];
        e->GetErrorMessage(szErrorMessage, _countof(szErrorMessage));
        FMLogOutputDebugString(szErrorMessage);
    }
    catch (CFileException* e)
    {
        nRet = eLogWriterExitCode_ThreadFunctionFileException;
        ASSERT(FALSE);
        e->ReportError();

        TCHAR szErrorMessage[512];
        e->GetErrorMessage(szErrorMessage, _countof(szErrorMessage));
        FMLogOutputDebugString(szErrorMessage);
    }
    catch (CException* e)
    {
        nRet = eLogWriterExitCode_ThreadFunctionException;
        ASSERT(FALSE);
        e->ReportError();

        TCHAR szErrorMessage[512];
        e->GetErrorMessage(szErrorMessage, _countof(szErrorMessage));
        FMLogOutputDebugString(szErrorMessage);
    }
    catch (...)
    {
        nRet = eLogWriterExitCode_ThreadFunctionUnknownException;
        ASSERT(FALSE);
        FMLogOutputDebugString(_T("Unknown exception is caught when writing a log to file.\n"));
    }

    ASSERT(nRet == eLogWriterExitCode_OK);
    FMLogOutputDebugString(_T("Log writer thread exit with code %d.\n"), nRet);

    return nRet;
}

void CFMLogWriter::AddLogToQueue(const CString& strErrMsg, const CString strUser, const CString& strModule, const CString& strLevel
    , LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction
    )
{
    while (m_logQueue.IsFull())
    {
        FMLogOutputDebugString(_T("Log queue is full. Count: %d.\n"), m_logQueue.Count());
        m_WriteEvent.SetEvent();
        Sleep(200);
    }

    CFMLogItem* pLogItem = new CFMLogItem();
    pLogItem->SetLogMsg(strErrMsg, strUser, strModule, strLevel, strSourceCodeFile, nSourceCodeLine, strSourceCodeFunction);
    m_logQueue.PushBack(pLogItem);
    
    //notify the writer thread that new log item is added.
    m_WriteEvent.SetEvent();
}

UINT CFMLogWriter::WriteThreadProc()
{
    FMLogLog(_T("CFMLogWriter::WriteThreadProc() begin"));

#if DEBUG_LOG_WRITER
    FMLogOutputDebugString(_T("Waiting write event ...\n"));
#endif // DEBUG_LOG_WRITER
    while (m_WriteEvent.Lock()) //waiting until new log item is added to queue.
    {
        do
        {
            CFMLogItem* pLogItem = m_logQueue.PopFront();
            if (pLogItem != nullptr)
            {
                int nRet = WriteToFile(pLogItem);
                ASSERT(nRet == 0);

                delete pLogItem;
            }
            else
            {
                break;
            }
        } while (1);
        
        if (m_bExitWriteThread)
        {
            if (m_logQueue.IsEmpty())
            {
                m_logThreadState = eLogThreadIsFinished;
                break;
            }
            else
            {
                //it should continue to write ... 
                m_WriteEvent.SetEvent();
            }
        }
    }

    ASSERT(m_logQueue.IsEmpty());
    
    CString strLogMsg;
    size_t nCountOfLogLeft = m_logQueue.Count();
    strLogMsg.Format(_T("CFMLogWriter::WriteThreadProc() end when the log queue size is %d."), nCountOfLogLeft);
    FMLogLog(strLogMsg);

    return 0;
}

int CFMLogWriter::WriteToFile(CFMLogItem* pLogItem)
{
    int nRet = eLogWriterExitCode_WriteToFileUnkownError;

    try
    {
        nRet = WriteToFileInternal(pLogItem);
        ASSERT(nRet == 0);
    }
    catch (CMemoryException* e)
    {
        nRet = eLogWriterExitCode_WriteToFileMemoryException;
        ASSERT(FALSE);
        e->ReportError();

        TCHAR szErrorMessage[512];
        e->GetErrorMessage(szErrorMessage, _countof(szErrorMessage));
        FMLogOutputDebugString(szErrorMessage);
    }
    catch (CFileException* e)
    {
        nRet = eLogWriterExitCode_WriteToFileFileException;
        ASSERT(FALSE);
        e->ReportError();

        TCHAR szErrorMessage[512];
        e->GetErrorMessage(szErrorMessage, _countof(szErrorMessage));
        FMLogOutputDebugString(szErrorMessage);
    }
    catch (CException* e)
    {
        nRet = eLogWriterExitCode_WriteToFileException;
        ASSERT(FALSE);
        e->ReportError();

        TCHAR szErrorMessage[512];
        e->GetErrorMessage(szErrorMessage, _countof(szErrorMessage));
        FMLogOutputDebugString(szErrorMessage);
    }
    catch (...)
    {
        nRet = eLogWriterExitCode_WriteToFileUnknownException;
        ASSERT(FALSE);
        FMLogOutputDebugString(_T("Unknown exception is caught when writing a log to file.\n"));
    }

    return nRet;
}

UINT CFMLogWriter::WriteToFileInternal(CFMLogItem* pLogItem)
{
    CString& strLogFile = m_strLogFileName;

    //CStdioFileEx oFile;
    CStdioFile oFile;
    CFileException ex;
    int iCounter = 0;//count for loop to avoid dead loop
    do
    {
        ASSERT(m_pLogFileMutex != nullptr);
        CSingleLock oLogFileLock(m_pLogFileMutex, TRUE);

        if (oFile.Open(strLogFile, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::shareDenyWrite, &ex))
        {
            oFile.SeekToEnd();

            WriteToFileInternal(pLogItem, oFile);

            //is there log item in the queue?
            if (oFile.GetLength() < m_nMaxSizeByteOfLogFile)
            {
                pLogItem = m_logQueue.PopFront();
                while (pLogItem != nullptr)
                {
                    WriteToFileInternal(pLogItem, oFile);
                    delete pLogItem;
                    if (oFile.GetLength() > m_nMaxSizeByteOfLogFile)
                    {
                        break;
                    }

                    pLogItem = m_logQueue.PopFront();
                }
            }

            ULONGLONG nFileLen = oFile.GetLength();
            oFile.Close();

            //
            //make sure the log file size is too big.
            //
            {
                if (nFileLen > m_nMaxSizeByteOfLogFile)
                {
                    CString strNewFileName = GenerateBackupLogFileName();
                    oFile.Rename(strLogFile, strNewFileName);
                }
            }

            return eLogWriterExitCode_OK;
        }
        else
        {
            if (ex.m_cause == CFileException::sharingViolation)
            {
                if (++iCounter > 100)
                {
                    ASSERT(FALSE);
                    FMLogOutputDebugString(_T("Failed to open the log file %s. Log missed.\n"), strLogFile);
                    return eLogWriterExitCode_OpenLogFileFailed_SharingViolation; //if this thread run 100 times then exit
                }

                FMLogOutputDebugString(_T("Failed to open the log file %s. Failed count:%d\n"), strLogFile, iCounter);

                Sleep(20);
                continue;
            }
            else
            {
                ASSERT(FALSE);

                TCHAR szCause[512];
                CString strFormatted;
                ex.GetErrorMessage(szCause, _countof(szCause));

                strFormatted.Format(_T("Failed to open the log file %s: "), strLogFile);
                strFormatted += szCause;

                FMLogOutputDebugString(strFormatted);

                return eLogWriterExitCode_OpenLogFileFailed;
            }
        }
    } 
    while (true);
}

void CFMLogWriter::WriteToFileInternal(CFMLogItem* pLogItem, CStdioFile& oFile)
{
    const CString& strLogMsg = pLogItem->GetLogMsg();
    
#if DEBUG_LOG_WRITER
    static size_t nMaxCountOfLogQueue = 0;
    size_t nCurCountOfLogQueue = m_logQueue.Count();
    nMaxCountOfLogQueue = nCurCountOfLogQueue > nMaxCountOfLogQueue ? nCurCountOfLogQueue : nMaxCountOfLogQueue;

    static size_t nNumberOfLog = 0;
    ++nNumberOfLog;

    SYSTEMTIME st;
    GetLocalTime(&st);
    CString strDateTime;
    strDateTime.Format(_T("%04d%02d%02d_%02d:%02d:%02d.%03d"),
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
        );

    CString strLogWriterDebugInfo;
    strLogWriterDebugInfo.Format(_T("[Max:%9d][Cur:%9d][No. %9d][%s] ")
        , nMaxCountOfLogQueue, nCurCountOfLogQueue, nNumberOfLog, strDateTime);

    oFile.WriteString(strLogWriterDebugInfo + strLogMsg);
#else
    oFile.WriteString(strLogMsg);
#endif
    
    //TRACE(_T("%s\n"), strLogMsg);

}

void CFMLogWriter::WriteLogOfLogWriter(const CString& strMsg, LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction)
{
    CFMLogItem logItem;
    CString strLogModuleName(_T("FMLogSys"));
    CString strUser = strLogModuleName; //log internal, we don't know current FM user name.
    
    Write(strMsg, strUser, strLogModuleName, CFMLogLevel::To3LetterString(eFMLogLevelSpecialInfo)
        , strSourceCodeFile, nSourceCodeLine, strSourceCodeFunction);
}

void CFMLogWriter::SetLogFileMaxSize(size_t nMaxSizeMegabyte)
{
    m_nMaxSizeByteOfLogFile = nMaxSizeMegabyte << 20;
}

void CFMLogWriter::SetLogFileName(CString val)
{
    m_strLogFileName = val;

    delete m_pLogFileMutex;
    m_pLogFileMutex = new CMutex(0, m_strLogFileName);
}

void CFMLogWriter::EndWriterThread()
{
    CString strLogMsg;
    strLogMsg.Format(_T("CFMLogWriter::EndWriterThread() begin when m_bExitWriteThread = %d"), m_bExitWriteThread);
    FMLogLog(strLogMsg);

    if (m_logThreadState == eLogThreadIsRunning)
    {
        ASSERT(m_pWriteThread != nullptr);

        m_bExitWriteThread = true;
        m_WriteEvent.SetEvent(); //wake the write thread to exit.
        WaitForSingleObject(m_pWriteThread->m_hThread, INFINITE); //waiting until the writer thread is finished.
        ASSERT(m_logThreadState == eLogThreadIsFinished);
        if (m_logThreadState != eLogThreadIsFinished)
        {
            FMLogOutputDebugString(_T("FM Log writer thread exited with unknown error.\n"));
            m_logThreadState = eLogThreadIsFinishedWithUnknowError;
        }

        m_pWriteThread = nullptr;
        ASSERT(m_logQueue.IsEmpty()); //assert there is no log in the queue.

        ASSERT(m_pLogFileMutex != nullptr);
        delete m_pLogFileMutex;
        m_pLogFileMutex = nullptr;
    }
    else
    {
        FMLogOutputDebugString(_T("FM Log writer thread have already exited.\n"));
    }
}

void CFMLogWriter::BeginWriterThread()
{
    FMLogLog(_T("================================================"));
    FMLogLog(_T("CFMLogWriter::BeginWriterThread() begin"));

    ASSERT(m_pWriteThread == nullptr);
    ASSERT(m_logThreadState == eLogThreadIsNotStart);

    if (m_logThreadState == eLogThreadIsNotStart)
    {
        m_WriteEvent.ResetEvent();
        SetLogFileMaxSize(MaxSizeMegabyteOfLogFile);

        m_pWriteThread = AfxBeginThread(CFMLogWriter::ThreadProc, (void *)this);
        m_logThreadState = eLogThreadIsRunning;

#ifdef _DEBUG
        m_pWriteThread->SuspendThread();
        FMSetThreadName(::GetCurrentThreadId(), "FMLogWriter");
        m_pWriteThread->ResumeThread();
#endif // _DEBUG
    }

    FMLogLog(_T("===============FM Log Thread Start=============="));
    FMLogLog(_T("CFMLogWriter::BeginWriterThread() end. <FirstLog>"));
}

bool CFMLogWriter::IsLogWriterThreadFinished()
{
    bool bRet = false;
    
    if (m_logThreadState != eLogThreadIsFinished)
    {
        bRet = true;
        
#if DEBUG_LOG_WRITER
        if (m_pWriteThread != nullptr && (!m_bExitWriteThread))
        {
            DWORD dwResult = WaitForSingleObject(m_pWriteThread->m_hThread, 0);
            if (WAIT_OBJECT_0 == dwResult)
            {
                //The thread is already done.
                VERIFY(FALSE);
                FMLogOutputDebugString(_T("CFMLogWriter::IsWriterThreadActive(), The thread is already done. But m_pWriteThread doesn't free.\n"));
            }
            else if (WAIT_TIMEOUT == dwResult)
            {
                //The thread is not exit.
                VERIFY(TRUE);
            }
            else
            {
                //The thread is error state.
                VERIFY(FALSE);
                FMLogOutputDebugString(_T("CFMLogWriter::IsWriterThreadActive(), The thread is in error state. But m_pWriteThread doesn't free.\n"));
            }
        }
#endif // DEBUG_LOG_WRITER
    }

    return bRet;
}

_FMLogNamespaceEnd
