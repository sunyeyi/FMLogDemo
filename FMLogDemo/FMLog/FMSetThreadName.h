///@file FMSetThreadName.h
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

#ifndef FMSetThreadName_h__
#define FMSetThreadName_h__
#pragma once

#include <wtypes.h>
#include "FMLogNamespaceMacro.h"


_FMLogNamespaceBegin

void FMSetThreadName(DWORD dwThreadID, char* threadName);

_FMLogNamespaceEnd

#endif // FMSetThreadName_h__

///@}//end of addtogroup FMLog FMLog
