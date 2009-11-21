#ifndef AESALON_INTERFACE_GDB_STRING_HANDLER_H
#define AESALON_INTERFACE_GDB_STRING_HANDLER_H

#include "misc/SmartPointer.h"
#include "String.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class Controller;

class StringHandler {
private:
    Misc::SmartPointer<Controller> controller;
public:
    StringHandler(Misc::SmartPointer<Controller> controller) : controller(controller) {}
    virtual ~StringHandler() {}
    
    virtual bool handle(Misc::SmartPointer<String> string) = 0;
    
    Misc::SmartPointer<Controller> get_controller() const { return controller; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
