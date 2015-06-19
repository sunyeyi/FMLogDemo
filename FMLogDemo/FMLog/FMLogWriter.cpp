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

#include "stdafx.h"
#include "FMLogWriter.h"
#include "FMLogItem.h"
#include "FMLogLevel.h"
#include "FMOutputDebugString.h"
#include "FMLogNamespaceMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEBUG_LOG_WRITER 0

_FMLogNamespaceBegin

//default log file size: 20M 
#define MaxSizeMegabyteOfLogFile 100

static LPCTSTR szLogFileName = _T("FMLog.txt");

CFMLogWriter::CFMLogWriter()
    : m_bExitWriteThread(false)
    , m_logQueue(10000)
    , m_pLogFileMutex(nullptr)
{
    SetLogFileName(szLogFileName);

    m_WriteEvent.ResetEvent();
    m_pWriteThread = AfxBeginThread(CFMLogWriter::ThreadProc, (void *)this);

    SetLogFileMaxSize(MaxSizeMegabyteOfLogFile);
}

CFMLogWriter::~CFMLogWriter()
{
    m_bExitWriteThread = true;
    Sleep(100);
    m_WriteEvent.SetEvent(); //wake the write thread to exit.
    WaitForSingleObject(m_pWriteThread->m_hThread, INFINITE); //waiting until the writer thread is finished.
    ASSERT(m_logQueue.IsEmpty()); //assert there is no log in the queue.
    
    ASSERT(m_pLogFileMutex != nullptr);
    delete m_pLogFileMutex;
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
    AddLogToQueue(strErrMsg, strUser, strModule, strLevel
        , strSourceCodeFile, nSourceCodeLine, strSourceCodeFunction);
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

    } while (PathFileExists(strBackupLogFileName));

    return strBackupLogFileName;
}

CFMLogWriter& CFMLogWriter::GetInstance()
{
    static CFMLogWriter theLogWriter;
    return theLogWriter;
}

UINT CFMLogWriter::ThreadProc(LPVOID lpParam)
{
    CFMLogWriter* pLogWriter = (CFMLogWriter*)lpParam;
    UINT nRet = pLogWriter->WriteThreadProc();
    return nRet;
}

void CFMLogWriter::AddLogToQueue(const CString& strErrMsg, const CString strUser, const CString& strModule, const CString& strLevel
    , LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction
    )
{
    CFMLogItem* pLogItem = new CFMLogItem();
    pLogItem->SetLogMsg(strErrMsg, strUser, strModule, strLevel, strSourceCodeFile, nSourceCodeLine, strSourceCodeFunction);

    while (m_logQueue.IsFull())
    {
        m_WriteEvent.SetEvent();
        Sleep(100);
    }
    
    m_logQueue.PushBack(pLogItem);
    
    //notify the writer thread that new log item is added.
    m_WriteEvent.SetEvent();
}

UINT CFMLogWriter::WriteThreadProc()
{
    WriteLogOfLogThread(true);

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
            break;
        }
    }

    ASSERT(m_logQueue.IsEmpty());

    WriteLogOfLogThread(false);
    return 0;
}

int CFMLogWriter::WriteToFile(CFMLogItem* pLogItem)
{
    int nRet = -1;

    try
    {
        nRet = WriteToFileInternal(pLogItem);
        ASSERT(nRet == 0);
    }
    catch (CMemoryException* e)
    {
        nRet = -4;
        ASSERT(FALSE);
        e->ReportError();
    }
    catch (CFileException* e)
    {
        nRet = -3;
        ASSERT(FALSE);
        e->ReportError();
    }
    catch (CException* e)
    {
        nRet = -2;
        ASSERT(FALSE);
        e->ReportError();
    }
    catch (...)
    {
        nRet = -1;
        ASSERT(FALSE);
        FMOutputDebugString(_T("Unknown exception is caught when writing a log to file."));
    }

    return nRet;
}

int CFMLogWriter::WriteToFileInternal(CFMLogItem* pLogItem)
{
    CString& strErrorLogFile = m_strLogFileName;
    //CStdioFileEx oFile;
    CStdioFile oFile;
    CFileException ex;
    int iCounter = 0;//count for loop to avoid dead loop
    do
    {
        ASSERT(m_pLogFileMutex != nullptr);
        CSingleLock oLogFileLock(m_pLogFileMutex, TRUE);

        if (oFile.Open(strErrorLogFile, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::shareDenyWrite, &ex))
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
                    oFile.Rename(strErrorLogFile, strNewFileName);
                }
            }

            return 0;
        }
        else
        {
            if (ex.m_cause == CFileException::sharingViolation)
            {
                if (++iCounter > 100)
                {
                    ASSERT(FALSE);
                    FMOutputDebugString(_T("Failed to open the log file. Log missed."));
                    return -1; //if this thread run 100 times then exit
                }

                FMOutputDebugString(_T("Failed to open the log file. Failed count:%d"), iCounter);

                Sleep(20);
                continue;
            }
            else
            {
                ASSERT(FALSE);
                FMOutputDebugString(_T("Failed to open the log file. Cause:%d"), ex.m_cause);
                return -1;
            }
        }

    } while (true);
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
    DWORD nThreadID = ::GetCurrentThreadId();
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

void CFMLogWriter::WriteLogOfLogThread(bool bStart)
{
    CFMLogItem logItem;
    CString strLogModuleName(_T("FMLogSys"));
    if (bStart) // log thread start
    {
        logItem.SetLogMsg(_T("\n"));
        WriteToFile(&logItem);

        logItem.SetLogMsg(_T("FM logger thread has been started."), strLogModuleName, strLogModuleName
            , CFMLogLevel::To3LetterString(eFMLogLevelInfo), __FILE__, __LINE__, __FUNCTION__);
        WriteToFile(&logItem);
    }
    else // log thread end
    {
        CString strLogMsg;
        size_t nCountOfLogLeft = m_logQueue.Count();
        strLogMsg.Format(_T("FM logger thread is ready to exit(%d)."), nCountOfLogLeft);
        logItem.SetLogMsg(strLogMsg, strLogModuleName, strLogModuleName
            , CFMLogLevel::To3LetterString(eFMLogLevelInfo), __FILE__, __LINE__, __FUNCTION__);
        WriteToFile(&logItem);
    }
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

_FMLogNamespaceEnd
