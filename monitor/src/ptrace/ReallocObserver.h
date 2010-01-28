#ifndef AESALON_MONITOR_PTRACE_REALLOC_OBSERVER_H
#define AESALON_MONITOR_PTRACE_REALLOC_OBSERVER_H

#include "BreakpointObserver.h"





class ReallocObserver : public BreakpointObserver {
public:
    virtual void handle_breakpoint(const BreakpointReference &breakpoint);
};





#endif
