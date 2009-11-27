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
    String::string_type_e interested_type;
public:
    StringObserver(String::string_type_e interested_type)
        : interested_type(interested_type) {}
    virtual ~StringObserver() {}
    
    virtual bool notify(Misc::SmartPointer<String> string, Misc::SmartPointer<StateManager> state_manager) = 0;
    
    String::string_type_e get_interested_type() const { return interested_type; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
