#ifndef AESALON_PLATFORM_PIPE_EXCEPTION_H
#define AESALON_PLATFORM_PIPE_EXCEPTION_H

#include <string>

#include "misc/Exception.h"

namespace Aesalon {
namespace Platform {

class PipeException : public Misc::Exception {
public:
    PipeException(std::string message) : Misc::Exception(message) {}
};

}  // namespace Platform
}  // namespace Aesalon

#endif
