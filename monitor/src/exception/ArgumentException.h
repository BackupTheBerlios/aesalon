#ifndef AESALON_EVENT_EXCEPTION_H
#define AESALON_EVENT_EXCEPTION_H

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

class NoArgumentToArgumentException : public ArgumentException {
public:
    NoArgumentToArgumentException(std::string which) : ArgumentException(Misc::StreamAsString()
        << "Argument expects argument: " << which) {}
    NoArgumentToArgumentException(char which) : ArgumentException(Misc::StreamAsString()
        << "Argument expects argument: -" << which) {}
};

class UnknownArgumentException : public ArgumentException {
public:
    UnknownArgumentException(std::string what) : ArgumentException(Misc::StreamAsString()
        << "Unknown argument encountered: \"" << what << "\"") {}
    UnknownArgumentException(char what) : ArgumentException(Misc::StreamAsString()
        << "Unknown short-form argument: \'" << what << "\'") {}
};

} // namespace Exception

#endif
