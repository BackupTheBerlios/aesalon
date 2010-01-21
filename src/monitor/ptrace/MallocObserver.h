#ifndef AESALON_MONITOR_PTRACE_MALLOC_OBSERVER_H
#define AESALON_MONITOR_PTRACE_MALLOC_OBSERVER_H

#include <set>

#include "BreakpointObserver.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class MallocObserver : public BreakpointObserver {
public:
    virtual void handle_breakpoint(const BreakpointReference &breakpoint);
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
