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
