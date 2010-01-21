#ifndef AESALON_MONITOR_PTRACE_FREE_OBSERVER_H
#define AESALON_MONITOR_PTRACE_FREE_OBSERVER_H

#include "BreakpointObserver.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class FreeObserver : public BreakpointObserver {
public:
    virtual bool handle_breakpoint(const BreakpointReference &breakpoint);
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
