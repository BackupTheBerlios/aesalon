#include "StateManager.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

void StateManager::notify(State from, State to) {
    observer_list_t::const_iterator i = observer_list.begin();
    for(; i != observer_list.end(); i ++) {
        if((*i)->notify(from, to)) return;
    }
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
