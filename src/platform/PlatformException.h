#ifndef AESALON_PLATFORM_PLATFORM_EXCEPTION_H
#define AESALON_PLATFORM_PLATFORM_EXCEPTION_H

#include <string>

#include "misc/Exception.h"

namespace Aesalon {
namespace Platform {

class PlatformException : public Misc::Exception {
public:
    PlatformException(std::string message);
};

}  // namespace Platform
}  // namespace Aesalon

#endif
