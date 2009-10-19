#ifndef AESALON_NULL_POINTER_EXCEPTION_H
#define AESALON_NULL_POINTER_EXCEPTION_H

#include <string>

#include "Exception.h"
#include "StreamAsString.h"

namespace Aesalon {
namespace Misc {

/** Exception class, thrown when a NULL SmartPointer is dereferenced.
*/
class NullPointerException : public Exception {
public:
    NullPointerException(std::string message) :
        Exception(StreamAsString() << "Null pointer exception: " << message) {
    }
};

} // namespace Misc
} // namespace Aesalon

#endif
