#ifndef AESALON_INTERFACE_GDB_STRING_HANDLER_H
#define AESALON_INTERFACE_GDB_STRING_HANDLER_H

#include "misc/SmartPointer.h"
#include "String.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StringHandler {
public:
    StringHandler() {}
    virtual ~StringHandler() {}
    
    virtual bool handle(Misc::SmartPointer<String> string) = 0;
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
