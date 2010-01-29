#ifndef AESALON_MONITOR_PTRACE_BREAKPOINT_OBSERVER_H
#define AESALON_MONITOR_PTRACE_BREAKPOINT_OBSERVER_H

namespace PTrace {

class Breakpoint;

class BreakpointObserver {
public:
    BreakpointObserver() {}
    virtual ~BreakpointObserver() {}
    
    virtual void handle_breakpoint(Breakpoint *breakpoint) = 0;
};

} // namespace PTrace

#endif
