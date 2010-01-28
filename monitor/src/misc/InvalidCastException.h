#ifndef AESALON_MISC_INVALID_CAST_EXCEPTION_H
#define AESALON_MISC_INVALID_CAST_EXCEPTION_H

#include "Exception.h"

namespace Aesalon {
namespace Misc {

class InvalidCastException : public Exception {
public:
    InvalidCastException() : Exception("Invalid cast") {}
};

} // namespace Misc
} // namespace Aesalon

#endif