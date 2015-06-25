///@file FMLoggerInterface.cpp
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
#include "FMLoggerInterface.h"
#include "FMLogNamespaceMacro.h"
#include "FMLogWriter.h"
#include "FMLoggerManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

FMLOG_MODULE_IMPLEMENT(FMGlobal);
FMLOG_MODULE_IMPLEMENT(FMAssert);

_FMLogNamespaceBegin

void SetLogLevel(EFMLogLevel eLogLevel)
{
    if (eLogLevel == eFMLogLevelOff)
    {
        ASSERT(FALSE); //todo
    }

    CFMLoggerManager::GetInstance().SetLevelForAllLogger(eLogLevel);
}

void FMLogEnd(const CString& strMessage, LPCSTR strSourceCodeFile, int nSourceCodeLine, LPCSTR strSourceCodeFunction)
{
    CString strMsg = _T("FMLogEnd,") + strMessage;
    CFMLogWriter::GetInstance().WriteLogOfLogWriter(strMsg, strSourceCodeFile, nSourceCodeLine, strSourceCodeFunction);

    CFMLogWriter::GetInstance().EndWriterThread();
}

_FMLogNamespaceEnd
