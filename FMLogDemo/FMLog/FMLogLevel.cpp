///@file FMLogLevel.cpp
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
#include "FMLogLevel.h"
#include "FMLogNamespaceMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

_FMLogNamespaceBegin

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

_FMLogNamespaceEnd
