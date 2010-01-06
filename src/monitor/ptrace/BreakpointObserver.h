#ifndef AESALON_MONITOR_PTRACE_BREAKPOINT_OBSERVER_H
#define AESALON_MONITOR_PTRACE_BREAKPOINT_OBSERVER_H

#include "Breakpoint.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class BreakpointObserver {
public:
    BreakpointObserver() {}
    virtual ~BreakpointObserver() {}
    
    virtual bool handle_breakpoint(Misc::SmartPointer<Breakpoint> breakpoint) = 0;
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
