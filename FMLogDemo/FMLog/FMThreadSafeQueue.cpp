///@file FMThreadSafeQueue.cpp
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
#include "FMThreadSafeQueue.h"
#include "FMOutputDebugString.h"
#include "FMLogNamespaceMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

_FMLogNamespaceBegin

CFMThreadSafeLogQueue::CFMThreadSafeLogQueue(int nMax)
    : m_nMax(nMax)
{

}

CFMThreadSafeLogQueue::~CFMThreadSafeLogQueue()
{

}

bool CFMThreadSafeLogQueue::IsEmpty()
{
    CSingleLock logQueueLock(&m_theQuenuCriticalSection, TRUE);

    return m_theQuenu.empty();
}

void CFMThreadSafeLogQueue::PushBack(CFMLogItem* pItem)
{
    CSingleLock logQueueLock(&m_theQuenuCriticalSection, TRUE);

    m_theQuenu.push(pItem);
}

CFMLogItem* CFMThreadSafeLogQueue::PopFront()
{
    CSingleLock logQueueLock(&m_theQuenuCriticalSection, TRUE);

    CFMLogItem* pRetItem = nullptr;

    if (!m_theQuenu.empty())
    {
        pRetItem = m_theQuenu.front();
        m_theQuenu.pop();
    }

    return pRetItem;
}

size_t CFMThreadSafeLogQueue::Count()
{
    CSingleLock logQueueLock(&m_theQuenuCriticalSection, TRUE);

    return m_theQuenu.size();
}

bool CFMThreadSafeLogQueue::IsFull()
{
    CSingleLock logQueueLock(&m_theQuenuCriticalSection, TRUE);

    bool bIsFull = (m_theQuenu.size() >= m_nMax);
    return bIsFull;
}

_FMLogNamespaceEnd
