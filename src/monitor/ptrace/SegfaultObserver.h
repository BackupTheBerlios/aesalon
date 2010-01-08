#ifndef AESALON_MONITOR_PTRACE_SEGFAULT_OBSERVER_H
#define AESALON_MONITOR_PTRACE_SEGFAULT_OBSERVER_H

#include "SignalObserver.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class SegfaultObserver : public SignalObserver {
public:
    virtual bool handle_signal(int signal, int status);
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
