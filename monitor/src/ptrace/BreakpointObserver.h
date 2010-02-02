#ifndef AESALON_MONITOR_PTRACE_BREAKPOINT_OBSERVER_H
#define AESALON_MONITOR_PTRACE_BREAKPOINT_OBSERVER_H

namespace PTrace {

class Breakpoint;

class BreakpointObserver {
public:
    BreakpointObserver() {}
    virtual ~BreakpointObserver() {}

    /** Process a breakpoint event.
        @param breakpoint The breakpoint that triggered itself.
        @return True if the observer is still valid.
    */
    virtual bool handle_breakpoint(Breakpoint *breakpoint) = 0;
};

} // namespace PTrace

#endif
