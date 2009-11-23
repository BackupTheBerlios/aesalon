#ifndef AESALON_INTERFACE_GDB_STRING_OBSERVER_MANAGER_H
#define AESALON_INTERFACE_GDB_STRING_OBSERVER_MANAGER_H

#include "String.h"
#include "StringObserver.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class StringObserverManager {
public:
    typedef std::vector<Misc::SmartPointer<StringObserver> > observer_vector_t;
private:
    observer_vector_t observer_vector;
public:
    StringObserverManager() {}
    virtual ~StringObserverManager() {}
    
    void add_observer(Misc::SmartPointer<StringObserver> observer) {
        observer_vector.push_back(observer);
    }
    
    void notify(Misc::SmartPointer<String> string) const;
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
