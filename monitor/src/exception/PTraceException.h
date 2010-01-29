#ifndef AESALON_MONITOR_PTRACE_PTRACE_EXCEPTION_H
#define AESALON_MONITOR_PTRACE_PTRACE_EXCEPTION_H

#include "BasicException.h"
#include "misc/StreamAsString.h"

namespace Exception {

class PTraceException : public BasicException {
public:
    PTraceException(std::string message) :
        BasicException(Misc::StreamAsString() << "PTrace exception: " << message) {}
    virtual ~PTraceException() {}
};

} // namespace Exception

#endif
