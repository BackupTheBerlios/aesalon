#ifndef AESALON_MONITOR_PTRACE_SIGNAL_OBSERVER_H
#define AESALON_MONITOR_PTRACE_SIGNAL_OBSERVER_H

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class SignalObserver {
public:
    SignalObserver() {}
    virtual ~SignalObserver() {}
    
    virtual bool handle_signal(int signal, int status) = 0;
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
