///@file FMUncopyable.h
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

#ifndef FMUncopyable_h__
#define FMUncopyable_h__
#include "FMLogNamespaceMacro.h"

#pragma once

_FMLogNamespaceBegin

class CFMUncopyable
{
protected:
    CFMUncopyable();
    ~CFMUncopyable();

private:
    CFMUncopyable(const CFMUncopyable&);
    CFMUncopyable& operator=(const CFMUncopyable&);
};

_FMLogNamespaceEnd

#endif // FMUncopyable_h__

///@}//end of addtogroup FMLog FMLog
