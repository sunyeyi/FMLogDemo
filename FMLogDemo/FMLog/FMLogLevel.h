///@file FMLogLevel.h
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

#ifndef FMLogLevel_h__
#define FMLogLevel_h__

#include "FMLogNamespaceMacro.h"

#pragma once

_FMLogNamespaceBegin

enum EFMLogLevel
{
    eFMLogLevelOff,
    eFMLogLevelSpecialInfo = -1,
    eFMLogLevelUnknown = 0,
    eFMLogLevelFatal = 1,
    eFMLogLevelError = 2,
    eFMLogLevelWarning = 3,
    eFMLogLevelInfo = 4,
    eFMLogLevelDebug = 5,
    eFMLogLevelTrace = 6,
    eFMLogLevelTemp = 7,
    eFMLogLevelAll = 9999,
};

class CFMLogLevel
{
public:
    CFMLogLevel();
    ~CFMLogLevel();

public:
    static LPCTSTR To3LetterString(EFMLogLevel eLogLevel);
};

_FMLogNamespaceEnd

#endif // FMLogLevel_h__

///@}//end of addtogroup FMLog FMLog
