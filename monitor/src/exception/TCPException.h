#ifndef AESALON_TCP_EXCEPTION_H
#define AESALON_TCP_EXCEPTION_H

#include <string>

#include "BasicException.h"
#include "misc/StreamAsString.h"

namespace Exception {

/** Exception class, thrown when the TCP library encounters a problem. */
class TCPException : public BasicException {
public:
    TCPException(std::string message) :
        BasicException(Misc::StreamAsString() << "TCP exception: " << message) {
    }
};

}

#endif
