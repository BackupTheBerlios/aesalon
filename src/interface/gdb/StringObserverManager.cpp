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

void StringObserverManager::notify(Misc::SmartPointer<String> string) const {
    observer_vector_t::const_iterator i = temporary_observers.begin();
    
    for(; i != temporary_observers.end(); i ++) {
        if((*i)->is_alive() && (*i)->notify(string)) break;
    }

    i = observer_vector.begin();
    
    for(; i != observer_vector.end(); i ++) {
        if((*i)->is_alive() && (*i)->notify(string)) break;
    }
}

void StringObserverManager::cleanup() {
    observer_vector_t::iterator i = temporary_observers.begin();
    
    /* TODO: make these loops more efficient */
    
    for(; i != temporary_observers.end(); i ++) {
        if((*i)->is_alive()) {
            temporary_observers.erase(i);
            i = temporary_observers.begin();
        }
    }

    i = observer_vector.begin();
    
    for(; i != observer_vector.end(); i ++) {
        if((*i)->is_alive()) {
            observer_vector.erase(i);
            i = observer_vector.begin();
        }
    }
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
