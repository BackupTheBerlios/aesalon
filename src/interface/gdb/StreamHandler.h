#ifndef AESALON_INTERFACE_GDB_STREAM_HANDLER_H
#define AESALON_INTERFACE_GDB_STREAM_HANDLER_H

#include "StringHandler.h"
#include "misc/SmartPointer.h"
#include "String.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StreamHandler : public StringHandler {
public:
    StreamHandler() {}
    virtual ~StreamHandler() {}
    
    virtual bool handle(Misc::SmartPointer<String> string) = 0;
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
