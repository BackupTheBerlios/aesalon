#ifndef AESALON_INTERFACE_GDB_STREAM_HANDLER_H
#define AESALON_INTERFACE_GDB_STREAM_HANDLER_H

#include "misc/SmartPointer.h"
#include "String.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StreamHandler {
public:
    StreamHandler() {}
    virtual ~StreamHandler() {}
    
    virtual void handle_stream(Misc::SmartPointer<StreamOutput> stream) = 0;
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
