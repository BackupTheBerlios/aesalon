#ifndef AESALON_INTERFACE_GDB_BREAKPOINT_SETUP_OBSERVER_H
#define AESALON_INTERFACE_GDB_BREAKPOINT_SETUP_OBSERVER_H

#include "StringObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class BreakpointSetupObserver : public StringObserver {
public:
    /* NOTE: these are hardcoded, perhaps not the best idea . . . */
    enum special_breakpoints_e {
        MALLOC = 2,
        FREE,
        REALLOC,
        CALLOC,
        LAST
    };

    BreakpointSetupObserver() : StringObserver() {}
    virtual ~BreakpointSetupObserver() {}
    
    virtual bool notify(Misc::SmartPointer<AsyncOutput> async);
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
