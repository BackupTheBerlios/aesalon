#ifndef AESALON_INTERFACE_GDB_EXIT_OBSERVER_H
#define AESALON_INTERFACE_GDB_EXIT_OBSERVER_H

#include "StringObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class ExitObserver : public StringObserver {
public:
    ExitObserver() : StringObserver(String::ASYNC_OUTPUT) {}
    virtual ~ExitObserver() {}
    
    virtual bool notify(Misc::SmartPointer<String> string, Misc::SmartPointer<StateManager> state_manager);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
