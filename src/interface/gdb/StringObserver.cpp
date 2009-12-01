#include "StringObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool StringObserver::notify(Misc::SmartPointer<String> string) {
    switch(string->get_type()) {
        case String::ASYNC_OUTPUT: {
            return notify(string.to<AsyncOutput>());
            break;
        }
        case String::RESULT_RECORD: {
            return notify(string.to<ResultRecord>());
            break;
        }
        case String::STREAM_OUTPUT: {
            return notify(string.to<StreamOutput>());
            break;
        }
    }
    return false; /* This should never be reached, but keep gcc happy. */
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
