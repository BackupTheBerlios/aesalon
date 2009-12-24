#ifndef AESALON_INTERFACE_PTRACE_EXIT_OBSERVER_H
#define AESALON_INTERFACE_PTRACE_EXIT_OBSERVER_H

#include "SignalObserver.h"

namespace Aesalon {
namespace Interface {
namespace PTrace {

class ExitObserver : public SignalObserver {
public:
    ExitObserver() : SignalObserver() {}
    virtual ~ExitObserver() {}
    
    virtual bool handle_signal(int signal, int status);
};

} // namespace PTrace
} // namespace Interface
} // namespace Aesalon

#endif
