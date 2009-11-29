#ifndef AESALON_INTERFACE_GDB_SETUP_STATE_OBSERVER_H
#define AESALON_INTERFACE_GDB_SETUP_STATE_OBSERVER_H

#include "StateObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class SetupStateObserver : public StateObserver {
public:
    SetupStateObserver() {}
    virtual ~SetupStateObserver() {}
    
    virtual bool notify(State from, State to);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
