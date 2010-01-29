#ifndef AESALON_MONITOR_PTRACE_MAIN_OBSERVER_H
#define AESALON_MONITOR_PTRACE_MAIN_OBSERVER_H

#include "BreakpointObserver.h"

namespace PTrace {

class MainObserver : public BreakpointObserver {
public:
    virtual void handle_breakpoint(Breakpoint *breakpoint);
};

} // namespace PTrace

#endif
