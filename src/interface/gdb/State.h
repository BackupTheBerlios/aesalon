#ifndef AESALON_INTERFACE_GDB_STATE_H
#define AESALON_INTERFACE_GDB_STATE_H

namespace Aesalon {
namespace Interface {
namespace GDB {

class State {
public:
    typedef enum {
        START, /**< GDB has been started, but nothing else has happened. */
        SETUP, /**< GDB is currently being set up, initialization is in progress. */
        RUNNING, /**< GDB is currently running. No commands can be executed during this state. */
        PAUSED, /**< GDB is paused, but has been running previously, and setup is complete. */
        FINISHED /**< GDB has finished executing the program. */
    } gdb_state;

private:
    gdb_state state;
public:
    State(gdb_state state) : state(state) {}
    virtual ~State() {}
    
    operator gdb_state() const { return get_state(); }
    gdb_state operator=(const gdb_state &new_state) { state = new_state; return state; }
    bool operator==(const gdb_state &other) const { return state == other; }
    bool operator!=(const gdb_state &other) const { return state != other; }
    
    gdb_state get_state() const { return state; }
    void set_state(gdb_state new_state) { state = new_state; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
