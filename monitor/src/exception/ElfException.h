#ifndef AESALON_MONITOR_ELF_EXCEPTION_H
#define AESALON_MONITOR_ELF_EXCEPTION_H

#include "BasicException.h"
#include "misc/StreamAsString.h"

namespace Exception {

class ElfException : public BasicException {
public:
    ElfException(std::string message) :
        BasicException(Misc::StreamAsString() << "Elf parser exception: " << message) {}
    virtual ~ElfException() {}
};

} // namespace Exception

#endif
