#ifndef AESALON_INTERFACE_PTRACE_PTRACE_EXCEPTION_H
#define AESALON_INTERFACE_PTRACE_PTRACE_EXCEPTION_H

#include "misc/Exception.h"

namespace Aesalon {
namespace Interface {
namespace PTrace {

class PTraceException : public Misc::Exception {
public:
    PTraceException(std::string message) : Misc::Exception(message) {}
    virtual ~PTraceException() {}
};

} // namespace PTrace
} // namespace Interface
} // namespace Aesalon

#endif
