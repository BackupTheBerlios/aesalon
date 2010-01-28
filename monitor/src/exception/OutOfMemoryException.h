#ifndef AESALON_MISC_OUT_OF_MEMORY_EXCEPTION_H
#define AESALON_MISC_OUT_OF_MEMORY_EXCEPTION_H

#include "BasicException.h"

namespace Exception {

/** Exception class, thrown when new or malloc() return NULL, denoting the
    lack of available memory on the heap. */
class OutOfMemoryException : public BasicException {
public:
    OutOfMemoryException() : BasicException("Couldn't allocate memory") {}
};

} // namespace Exception

#endif
