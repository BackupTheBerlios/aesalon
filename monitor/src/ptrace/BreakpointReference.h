#ifndef AESALON_MONITOR_PTRACE_BREAKPOINT_REFERENCE_H
#define AESALON_MONITOR_PTRACE_BREAKPOINT_REFERENCE_H

#
#include "Breakpoint.h"





class BreakpointReference : public Breakpoint *{
public:
    BreakpointReference(Breakpoint *breakpoint) : Misc::SmartPointer<Breakpoint>(breakpoint) {}
    virtual ~BreakpointReference() {}
};





#endif
