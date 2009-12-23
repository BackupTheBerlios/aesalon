#ifndef AESALON_INTERFACE_PTRACE_SIGNAL_OBSERVER_H
#define AESALON_INTERFACE_PTRACE_SIGNAL_OBSERVER_H

namespace Aesalon {
namespace Interface {

class PTraceSignalObserver {
public:
    PTraceSignalObserver() {}
    virtual ~PTraceSignalObserver() {}
    
    virtual bool handle_signal(int signal) = 0;
};

} // namespace Interface
} // namespace Aesalon

#endif
