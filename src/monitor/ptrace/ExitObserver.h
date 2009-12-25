#ifndef AESALON_MONITOR_PTRACE_EXIT_OBSERVER_H
#define AESALON_MONITOR_PTRACE_EXIT_OBSERVER_H

#include "SignalObserver.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class ExitObserver : public SignalObserver {
public:
    ExitObserver() : SignalObserver() {}
    virtual ~ExitObserver() {}
    
    virtual bool handle_signal(int signal, int status);
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
