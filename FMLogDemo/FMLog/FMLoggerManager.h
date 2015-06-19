///@file FMLoggerManager.h
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

#ifndef FMLoggerManager_h__
#define FMLoggerManager_h__

#pragma once
#include <map>
#include "FMUncopyable.h"
#include "FMLogNamespaceMacro.h"

_FMLogNamespaceBegin

class CFMLoggerManager : private CFMUncopyable
{
public:
    static CFMLoggerManager& GetInstance();
    ~CFMLoggerManager();
    void Add(const CString& strModule, CFMLogger* pLogger);
    void SetLevelForAllLogger(EFMLogLevel eLogLevel);

private:
    CFMLoggerManager();

private:
    std::map<CString, CFMLogger*> m_mapModuleToLoger; //keep all logger instances
};

_FMLogNamespaceEnd

#endif // FMLoggerManager_h__

///@}//end of addtogroup FMLog FMLog
