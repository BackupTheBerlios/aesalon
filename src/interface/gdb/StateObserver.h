#ifndef AESALON_INTERFACE_GDB_STATE_OBSERVER_H
#define AESALON_INTERFACE_GDB_STATE_OBSERVER_H

#include "State.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StateManager;

class StateObserver {
public:
    StateObserver() {}
    virtual ~StateObserver() {}
    
    virtual bool notify(State from, State to) = 0;
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
