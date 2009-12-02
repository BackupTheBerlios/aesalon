#include "BreakpointObserver.h"
#include "StringFollower.h"
#include "interface/Initializer.h"
#include "misc/String.h"
#include "BreakpointSetupObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool BreakpointObserver::notify(Misc::SmartPointer<AsyncOutput> async) {
    if(async->get_data()->get_first() == "stopped") {
        if(StringFollower(async).follow("'reason' rhs") == "breakpoint-hit") {
            int breakpoint_number;
            Misc::String::to<int>(StringFollower(async).follow("'bkptno' rhs"), breakpoint_number);
            if(breakpoint_number < BreakpointSetupObserver::LAST) return false;
            /* NOTE: update variable objects in here. */
            Initializer::get_instance()->get_controller()->send_command("-exec-continue");
            return true;
        }
    }
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
