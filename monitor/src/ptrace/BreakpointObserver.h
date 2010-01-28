#ifndef AESALON_MONITOR_PTRACE_BREAKPOINT_OBSERVER_H
#define AESALON_MONITOR_PTRACE_BREAKPOINT_OBSERVER_H

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class BreakpointReference;

class BreakpointObserver {
public:
    BreakpointObserver() {}
    virtual ~BreakpointObserver() {}
    
    virtual void handle_breakpoint(const BreakpointReference &breakpoint) = 0;
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
