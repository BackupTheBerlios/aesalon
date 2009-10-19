#ifndef AESALON_MISC_OUT_OF_MEMORY_EXCEPTION_H
#define AESALON_MISC_OUT_OF_MEMORY_EXCEPTION_H

#include "Exception.h"

namespace Aesalon {
namespace Misc {

/** Exception class, thrown when new or malloc() return NULL, denoting the
    lack of available memory on the heap. */
class OutOfMemoryException : public Exception {
public:
    OutOfMemoryException() : Exception("Couldn't allocate memory.") {}
};

} // namespace Misc
} // namespace Aesalon

#endif
