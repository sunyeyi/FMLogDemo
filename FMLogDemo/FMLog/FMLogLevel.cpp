#include "StdAfx.h"
#include "FMLogLevel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFMLogLevel::CFMLogLevel()
{
}


CFMLogLevel::~CFMLogLevel()
{
}

LPCTSTR CFMLogLevel::To3LetterString(EFMLogLevel eLogLevel)
{
    switch (eLogLevel)
    {
    case eFMLogLevelSpecialInfo:
        return _T("Spe");
    case eFMLogLevelUnknown:
        return _T("Unk");
    case eFMLogLevelFatal:
        return _T("Fat");
    case eFMLogLevelError:
        return _T("Err");
    case eFMLogLevelWarning:
        return _T("War");
    case eFMLogLevelInfo:
        return _T("Inf");
    case eFMLogLevelDebug:
        return _T("Deb");
    case eFMLogLevelTrace:
        return _T("Tra");
    case eFMLogLevelTemp:
        return _T("Tem");
    default:
        ASSERT(FALSE);
        return _T("!!!");
    }
}
