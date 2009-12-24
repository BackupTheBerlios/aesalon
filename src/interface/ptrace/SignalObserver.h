#ifndef AESALON_INTERFACE_PTRACE_SIGNAL_OBSERVER_H
#define AESALON_INTERFACE_PTRACE_SIGNAL_OBSERVER_H

namespace Aesalon {
namespace Interface {
namespace PTrace {

class SignalObserver {
public:
    SignalObserver() {}
    virtual ~SignalObserver() {}
    
    virtual bool handle_signal(int signal, int status) = 0;
};

} // namespace PTrace
} // namespace Interface
} // namespace Aesalon

#endif
