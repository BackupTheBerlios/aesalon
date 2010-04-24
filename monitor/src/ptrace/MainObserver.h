#ifndef AESALON_PTRACE_MAIN_OBSERVER_H
#define AESALON_PTRACE_MAIN_OBSERVER_H

#include "BreakpointObserver.h"

namespace PTrace {

class MainObserver : public BreakpointObserver {
public:
    MainObserver();
    virtual ~MainObserver();
    
    virtual bool handle_breakpoint(Breakpoint *breakpoint);
};

} // namespace PTrace

#endif
