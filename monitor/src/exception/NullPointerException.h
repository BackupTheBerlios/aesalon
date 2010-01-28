#ifndef AESALON_NULL_POINTER_EXCEPTION_H
#define AESALON_NULL_POINTER_EXCEPTION_H

#include <string>

#include "BasicException.h"
#include "misc/StreamAsString.h"

namespace Exception {

/** Exception class, thrown when a NULL pointer is found where it shouldn't be. */
class NullPointerException : public BasicException {
public:
    NullPointerException(std::string message) :
        BasicException(Misc::StreamAsString() << "Null pointer exception: " << message) {
    }
};

}

#endif
