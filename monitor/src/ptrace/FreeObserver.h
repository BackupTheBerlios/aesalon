#ifndef AESALON_MONITOR_PTRACE_FREE_OBSERVER_H
#define AESALON_MONITOR_PTRACE_FREE_OBSERVER_H

#include "BreakpointObserver.h"

namespace PTrace {

class FreeObserver : public BreakpointObserver {
public:
    virtual bool handle_breakpoint(Breakpoint *breakpoint);
};

} // namespace PTrace

#endif
