#include "StringObserverManager.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

void StringObserverManager::notify(Misc::SmartPointer<String> string, Misc::SmartPointer<StateManager> state_manager) const {
    observer_vector_t::const_iterator i = observer_vector.begin();
    
    for(; i != observer_vector.end(); i ++) {
        if((*i)->get_interested_type() == string->get_type() && (*i)->notify(string, state_manager)) break;
    }
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
