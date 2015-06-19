#ifndef FMUncopyable_h__
#define FMUncopyable_h__

#pragma once
class CFMUncopyable
{
protected:
    CFMUncopyable();
    ~CFMUncopyable();

private:
    CFMUncopyable(const CFMUncopyable&);
    CFMUncopyable& operator=(const CFMUncopyable&);
};

#endif // FMUncopyable_h__
