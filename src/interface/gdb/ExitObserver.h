#ifndef AESALON_INTERFACE_GDB_EXIT_OBSERVER_H
#define AESALON_INTERFACE_GDB_EXIT_OBSERVER_H

#include "StringObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class ExitObserver : public StringObserver {
public:
    ExitObserver(Misc::SmartPointer<StateManager> state_manager) : StringObserver(state_manager) {}
    virtual ~ExitObserver() {}
    
    virtual bool notify(Misc::SmartPointer<AsyncOutput> async);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
