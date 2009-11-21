#ifndef AESALON_INTERFACE_GDB_ASYNC_HANDLER_H
#define AESALON_INTERFACE_GDB_ASYNC_HANDLER_H

#include "misc/SmartPointer.h"
#include "String.h"
#include "StringHandler.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class AsyncHandler :public StringHandler {
public:
    AsyncHandler(Misc::SmartPointer<Controller> controller) : StringHandler(controller) {}
    virtual ~AsyncHandler() {}
    
    virtual bool handle(Misc::SmartPointer<String> string) {
        return handle(string.to<AsyncOutput>());
    }
    virtual bool handle(Misc::SmartPointer<AsyncOutput> string) = 0;
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
