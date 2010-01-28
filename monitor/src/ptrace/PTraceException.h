#ifndef AESALON_MONITOR_PTRACE_PTRACE_EXCEPTION_H
#define AESALON_MONITOR_PTRACE_PTRACE_EXCEPTION_H

#include "misc/Exception.h"





class PTraceException : public Misc::Exception {
public:
    PTraceException(std::string message) : Misc::Exception(message) {}
    virtual ~PTraceException() {}
};





#endif
