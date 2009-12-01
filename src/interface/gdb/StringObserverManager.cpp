#include "StringObserverManager.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

void StringObserverManager::remove_observer(Misc::SmartPointer<StringObserver> observer) {
    observer_vector_t::iterator i = temporary_observers.begin();
    for(; i != temporary_observers.end(); i ++) {
        if(observer == *i) {
            temporary_observers.erase(i);
            return;
        }
    }
    
    i = observer_vector.begin();
    for(; i != observer_vector.end(); i ++) {
        if(observer == *i) {
            observer_vector.erase(i);
            return;
        }
    }
}

void StringObserverManager::notify(Misc::SmartPointer<String> string, Misc::SmartPointer<StateManager> state_manager) const {
    observer_vector_t::const_iterator i = observer_vector.begin();
    
    for(; i != observer_vector.end(); i ++) {
        if((*i)->get_interested_type() == string->get_type() && (*i)->notify(string, state_manager)) break;
    }
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
