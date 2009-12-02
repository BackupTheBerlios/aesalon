#ifndef AESALON_INTERFACE_GDB_STRING_OBSERVER_H
#define AESALON_INTERFACE_GDB_STRING_OBSERVER_H

#include "misc/SmartPointer.h"
#include "String.h"
#include "StateManager.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StringObserver {
private:
    Misc::SmartPointer<StateManager> state_manager;
    std::size_t id;
    static std::size_t last_id; /**< Used to generate unique observer IDs. */
    bool alive;
protected:
    Misc::SmartPointer<StateManager> get_state_manager() const { return state_manager; }
public:
    StringObserver(Misc::SmartPointer<StateManager> state_manager) : state_manager(state_manager), id(last_id++), alive(true) {}
    virtual ~StringObserver() {}
    
    bool operator==(const StringObserver &other) const {
        if(id == other.id) return true;
        return false;
    }
    virtual bool notify(Misc::SmartPointer<String> string);
    virtual bool notify(Misc::SmartPointer<AsyncOutput> async) { return false; }
    virtual bool notify(Misc::SmartPointer<ResultRecord> result) { return false; }
    virtual bool notify(Misc::SmartPointer<StreamOutput> stream) { return false; }
    
    bool is_alive() const { return alive; }
    void set_alive(bool new_alive) { alive = new_alive; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
