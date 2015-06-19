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
