#include "BreakpointSetupObserver.h"
#include "StringFollower.h"
#include "interface/Initializer.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool BreakpointSetupObserver::notify(Misc::SmartPointer<AsyncOutput> async) {
    if(async->get_data()->get_first() == "stopped") return false;
    if(StringFollower(async).follow("'reason' rhs") != "breakpoint-hit") return false;
    if(StringFollower(async).follow("'bkptno' rhs") != "1") return false;
    else { /* FIXME: all of these names are hardcoded; not a good idea . . . */
        /* #2 */
        Initializer::get_instance()->get_controller()->send_command("-break-insert __malloc");
        /* #3 */
        Initializer::get_instance()->get_controller()->send_command("-break-insert __free");
        /* #4 */
        Initializer::get_instance()->get_controller()->send_command("-break-insert __realloc");
        /* #5 */
        Initializer::get_instance()->get_controller()->send_command("-break-insert __calloc");
        
        set_alive(false);
    }
    
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
