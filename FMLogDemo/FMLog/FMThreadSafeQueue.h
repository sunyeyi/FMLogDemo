#ifndef FMThreadSafeQueue_h__
#define FMThreadSafeQueue_h__

#pragma once
#include <queue>
#include "FMUncopyable.h"
#include "FMLogNamespaceMacro.h"

_FMLogNamespaceBegin

class CFMLogItem;

class CFMThreadSafeLogQueue : private CFMUncopyable
{
public:
    CFMThreadSafeLogQueue(int nMax);
    ~CFMThreadSafeLogQueue();

public:
    bool IsEmpty();
    bool IsFull();
    void PushBack(CFMLogItem* pItem);
    CFMLogItem* PopFront();
    size_t Count();

private:
    std::queue<CFMLogItem*> m_theQuenu;
    CCriticalSection m_theQuenuCriticalSection;
    size_t m_nMax;
};

_FMLogNamespaceEnd

#endif // FMThreadSafeQueue_h__
