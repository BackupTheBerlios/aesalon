#ifndef AESALON_MONITOR_PTRACE_BREAKPOINT_REFERENCE_H
#define AESALON_MONITOR_PTRACE_BREAKPOINT_REFERENCE_H

#include "misc/SmartPointer.h"
#include "Breakpoint.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class BreakpointReference : public Misc::SmartPointer<Breakpoint> {
public:
    BreakpointReference(Misc::SmartPointer<Breakpoint> breakpoint) : Misc::SmartPointer<Breakpoint>(breakpoint) {}
    virtual ~BreakpointReference() {}
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
