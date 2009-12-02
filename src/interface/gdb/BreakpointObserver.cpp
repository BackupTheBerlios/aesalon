#include "BreakpointObserver.h"
#include "StringFollower.h"
#include "interface/Initializer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool BreakpointObserver::notify(Misc::SmartPointer<AsyncOutput> async) {
    if(async->get_data()->get_first() == "stopped") {
        if(StringFollower(async).follow("'reason' rhs") == "breakpoint-hit") {
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
