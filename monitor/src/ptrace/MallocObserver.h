#ifndef AESALON_MONITOR_PTRACE_MALLOC_OBSERVER_H
#define AESALON_MONITOR_PTRACE_MALLOC_OBSERVER_H

#include <set>

#include "BreakpointObserver.h"





class MallocObserver : public BreakpointObserver {
public:
    virtual void handle_breakpoint(const BreakpointReference &breakpoint);
};





#endif
