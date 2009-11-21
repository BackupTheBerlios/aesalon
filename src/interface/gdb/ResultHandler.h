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
    ResultHandler(Misc::SmartPointer<Controller> controller) : StringHandler(controller) {}
    virtual ~ResultHandler() {}
    
    virtual bool handle(Misc::SmartPointer<String> string) {
        return handle(string.to<ResultRecord>());
    }
    virtual bool handle(Misc::SmartPointer<ResultRecord> result) = 0;
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
