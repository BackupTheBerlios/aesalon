#ifndef AESALON_EVENT_EXCEPTION_H
#define AESALON_EVENT_EXCEPTION_H

#include <string>

#include "BasicException.h"
#include "misc/StreamAsString.h"

namespace Exception {

/** Exception class, thrown when the event library encounters a problem. */
class EventException : public BasicException {
public:
    EventException(std::string message) :
        BasicException(Misc::StreamAsString() << "Event exception: " << message) {
    }
};

}

#endif
