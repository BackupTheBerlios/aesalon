#ifndef AESALON_INTERFACE_GDB_STATE_MANAGER_H
#define AESALON_INTERFACE_GDB_STATE_MANAGER_H

namespace Aesalon {
namespace Interface {
namespace GDB {

class StateManager {
public:
    enum gdb_state_e {
        SETUP, /**< GDB is currently being set up, initialization is in progress. */
        RUNNING, /**< GDB is currently running. No commands can be executed during this state. */
        PAUSED, /**< GDB is paused, but has been running previously, and setup is complete. */
        FINISHED, /**< GDB has finished executing the program. */
        UNKNOWN /**< GDB is in an unknown state; this shouldn't happen. */
    };
private:
    gdb_state_e state;
public:
    StateManager() : state(UNKNOWN) {}
    virtual ~StateManager() {}
    
    operator gdb_state_e() const { return get_state(); }
    
    gdb_state_e get_state() const { return state; }
    void set_state(gdb_state_e new_state) { state = new_state; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
