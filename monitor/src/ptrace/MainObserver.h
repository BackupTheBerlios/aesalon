#ifndef AESALON_MONITOR_PTRACE_MAIN_OBSERVER_H
#define AESALON_MONITOR_PTRACE_MAIN_OBSERVER_H

#include "BreakpointObserver.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class MainObserver : public BreakpointObserver {
public:
    virtual void handle_breakpoint(const BreakpointReference &breakpoint);
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
