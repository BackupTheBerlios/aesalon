#ifndef AESALON_INTERFACE_GDB_STATE_MANAGER_H
#define AESALON_INTERFACE_GDB_STATE_MANAGER_H

#include <vector>

#include "State.h"
#include "StateObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StateManager {
public:
    typedef std::vector<Misc::SmartPointer<StateObserver> > observer_list_t;
private:
    State state;
    observer_list_t observer_list;
    
    void notify(State from, State to);
public:
    StateManager() : state(State::UNKNOWN) {}
    virtual ~StateManager() {}
    
    operator State() const { return get_state(); }
    
    State get_state() const { return state; }
    void set_state(State new_state) { State old = state; state = new_state; notify(old, state); }
    
    void add_observer(Misc::SmartPointer<StateObserver> new_observer)
        { observer_list.push_back(new_observer); }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
