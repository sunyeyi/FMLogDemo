﻿///@file FMLoggerManager.cpp
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
#include "FMLogger.h"
#include "FMLoggerManager.h"
#include "FMLogNamespaceMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

_FMLogNamespaceBegin

CFMLoggerManager& CFMLoggerManager::GetInstance()
{
    static CFMLoggerManager theInstance;
    return theInstance;
}

CFMLoggerManager::CFMLoggerManager()
{

}

CFMLoggerManager::~CFMLoggerManager()
{

}

void CFMLoggerManager::Add(const CString& strModule, CFMLogger* pLogger)
{
    ASSERT(m_mapModuleToLoger.find(strModule) == m_mapModuleToLoger.end());
    m_mapModuleToLoger[strModule] = pLogger;
}

void CFMLoggerManager::SetLevelForAllLogger(EFMLogLevel eLogLevel)
{
    for (auto it = m_mapModuleToLoger.begin(); it != m_mapModuleToLoger.end(); ++it)
    {
        it->second->SetLogLevel(eLogLevel);
    }
}

_FMLogNamespaceEnd

