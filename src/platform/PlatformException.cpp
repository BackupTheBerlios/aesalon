#include <errno.h>
#include <cstring>
#include "misc/StreamAsString.h"

#include "PlatformException.h"

namespace Aesalon {
namespace Platform {
PlatformException::PlatformException(std::string message) :
    Misc::Exception(Misc::StreamAsString() << message << strerror(errno)) {
}

PlatformException::PlatformException(std::string message, bool no_errno) :
    Misc::Exception(message) {
}

}  // namespace Platform
}  // namespace Aesalon
