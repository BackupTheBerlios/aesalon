#ifndef AESALON_MONITOR_PTRACE_PTRACE_EXCEPTION_H
#define AESALON_MONITOR_PTRACE_PTRACE_EXCEPTION_H

#include "misc/Exception.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

class PTraceException : public Misc::Exception {
public:
    PTraceException(std::string message) : Misc::Exception(message) {}
    virtual ~PTraceException() {}
};

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon

#endif
