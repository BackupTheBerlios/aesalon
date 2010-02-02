#ifndef AESALON_MONITOR_PTRACE_MALLOC_OBSERVER_H
#define AESALON_MONITOR_PTRACE_MALLOC_OBSERVER_H

#include <set>

#include "BreakpointObserver.h"

namespace PTrace {

class MallocObserver : public BreakpointObserver {
public:
    virtual bool handle_breakpoint(Breakpoint *breakpoint);
};

} // namespace PTrace

#endif
