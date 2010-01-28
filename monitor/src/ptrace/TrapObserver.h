#ifndef AESALON_MONITOR_PTRACE_TRAP_OBSERVER_H
#define AESALON_MONITOR_PTRACE_TRAP_OBSERVER_H

#include "SignalObserver.h"





class TrapObserver : public SignalObserver {
public:
    TrapObserver() : SignalObserver() {}
    virtual ~TrapObserver() {}
    
    virtual bool handle_signal(int signal, int status);
};





#endif
