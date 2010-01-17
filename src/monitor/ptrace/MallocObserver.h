#ifndef AESALON_MONITOR_PTRACE_MALLOC_OBSERVER_H
#define AESALON_MONITOR_PTRACE_MALLOC_OBSERVER_H

#include <set>

#include "BreakpointObserver.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class MallocObserver : public BreakpointObserver {
protected:
    typedef std::set<std::size_t> breakpoint_set_t;
private:
    breakpoint_set_t breakpoints;
public:
    MallocObserver() {}
    virtual ~MallocObserver() {}
    virtual bool handle_breakpoint(const BreakpointReference &breakpoint);
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
