#ifndef AESALON_INTERFACE_PTRACE_SIGNAL_OBSERVER_H
#define AESALON_INTERFACE_PTRACE_SIGNAL_OBSERVER_H

namespace Aesalon {
namespace Interface {
namespace PTrace {

class PTraceSignalObserver {
public:
    PTraceSignalObserver() {}
    virtual ~PTraceSignalObserver() {}
    
    virtual bool handle_signal(int signal) = 0;
};

} // namespace PTrace
} // namespace Interface
} // namespace Aesalon

#endif
