#ifndef AESALON_INTERFACE_GDB_STATE_MANAGER_H
#define AESALON_INTERFACE_GDB_STATE_MANAGER_H

#include <vector>

#include "State.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StateManager {
private:
    State state;
public:
    StateManager() : state(State::START) {}
    virtual ~StateManager() {}
    
    operator State() const { return get_state(); }
    
    State get_state() const { return state; }
    void set_state(State new_state) { state = new_state; }
    
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
