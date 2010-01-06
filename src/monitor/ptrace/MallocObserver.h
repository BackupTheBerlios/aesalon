#ifndef AESALON_MONITOR_PTRACE_MALLOC_OBSERVER_H
#define AESALON_MONITOR_PTRACE_MALLOC_OBSERVER_H

#include "BreakpointObserver.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class MallocObserver : public BreakpointObserver {
public:
    MallocObserver() {}
    virtual ~MallocObserver() {}
    virtual bool handle_breakpoint(Misc::SmartPointer<Breakpoint> breakpoint);
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
