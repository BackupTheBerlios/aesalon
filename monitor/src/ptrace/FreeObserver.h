#ifndef AESALON_MONITOR_PTRACE_FREE_OBSERVER_H
#define AESALON_MONITOR_PTRACE_FREE_OBSERVER_H

#include "BreakpointObserver.h"





class FreeObserver : public BreakpointObserver {
public:
    virtual void handle_breakpoint(const BreakpointReference &breakpoint);
};





#endif
