#ifndef AESALON_EVENT_ARGUMENT_EXCEPTION_H
#define AESALON_EVENT_ARGUMENT_EXCEPTION_H

#include <string>

#include "BasicException.h"
#include "misc/StreamAsString.h"

namespace Exception {

/** Exception class, thrown when the event library encounters a problem. */
class ArgumentException : public BasicException {
public:
    ArgumentException(std::string message) :
        BasicException(Misc::StreamAsString() << "Argument exception: " << message) {
    }
};

} // namespace Exception

#endif
