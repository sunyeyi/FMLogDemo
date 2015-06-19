#ifndef FMLogLevel_h__
#define FMLogLevel_h__

#pragma once

enum EFMLogLevel
{
    eFMLogLevelSpecialInfo = -1, //output always but no error info.
    eFMLogLevelUnknown = 0,
    eFMLogLevelFatal = 1,
    eFMLogLevelError = 2,
    eFMLogLevelWarning = 3,
    eFMLogLevelInfo = 4,
    eFMLogLevelDebug = 5,
    eFMLogLevelTrace = 6,
    eFMLogLevelTemp = 7,
};

class CFMLogLevel
{
public:
    CFMLogLevel();
    ~CFMLogLevel();

public:
    static LPCTSTR To3LetterString(EFMLogLevel eLogLevel);
};

#endif // FMLogLevel_h__
