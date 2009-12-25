#ifndef AESALON_INTERFACE_PTRACE_TRAP_OBSERVER_H
#define AESALON_INTERFACE_PTRACE_TRAP_OBSERVER_H

#include "SignalObserver.h"

namespace Aesalon {
namespace Interface {
namespace PTrace {

class TrapObserver : public SignalObserver {
public:
    TrapObserver() : SignalObserver() {}
    virtual ~TrapObserver() {}
    
    virtual bool handle_signal(int signal, int status);
};

} // namespace PTrace
} // namespace Interface
} // namespace Aesalon

#endif
