#ifndef AESALON_MISC_OUT_OF_MEMORY_EXCEPTION_H
#define AESALON_MISC_OUT_OF_MEMORY_EXCEPTION_H

#include "Exception.h"

namespace Aesalon {
namespace Misc {

class OutOfMemoryException : public Exception {
public:
    OutOfMemoryException() : Exception("Couldn't allocate memory.") {}
};

} // namespace Misc
} // namespace Aesalon

#endif
