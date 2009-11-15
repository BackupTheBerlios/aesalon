#ifndef AESALON_INTERFACE_GDB_RESULT_HANDLER_H
#define AESALON_INTERFACE_GDB_RESULT_HANDLER_H

#include "misc/SmartPointer.h"
#include "String.h"
#include "StringHandler.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class ResultHandler : public StringHandler {
public:
    ResultHandler() {}
    virtual ~ResultHandler() {}
    
    virtual bool handle(Misc::SmartPointer<String> string) = 0;
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
