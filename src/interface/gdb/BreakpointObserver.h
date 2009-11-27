#ifndef AESALON_INTERFACE_GDB_BREAKPOINT_OBSERVER_H
#define AESALON_INTERFACE_GDB_BREAKPOINT_OBSERVER_H


#include "String.h"
#include "StringObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class BreakpointObserver : public StringObserver {
public:
    BreakpointObserver() : StringObserver(String::ASYNC_OUTPUT) {}
    virtual ~BreakpointObserver() {}
    
    virtual bool notify(Misc::SmartPointer<String> string, Misc::SmartPointer<StateManager> state_manager);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
