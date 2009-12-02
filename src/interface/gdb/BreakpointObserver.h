#ifndef AESALON_INTERFACE_GDB_BREAKPOINT_OBSERVER_H
#define AESALON_INTERFACE_GDB_BREAKPOINT_OBSERVER_H

#include "StringObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class BreakpointObserver : public StringObserver {
public:
    BreakpointObserver() : StringObserver() {}
    virtual ~BreakpointObserver() {}
    
    virtual bool notify(Misc::SmartPointer<AsyncOutput> async);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
