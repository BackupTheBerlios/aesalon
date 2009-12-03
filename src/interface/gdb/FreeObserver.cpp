#include "FreeObserver.h"
#include "StringFollower.h"
#include "interface/Initializer.h"
#include "misc/String.h"
#include "BreakpointSetupObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool FreeObserver::notify(Misc::SmartPointer<AsyncOutput> async) {
    if(async->get_data()->get_first() == "stopped") {
        if(StringFollower(async).follow("'reason' rhs") == "breakpoint-hit") {
            int breakpoint_number;
            Misc::String::to<int>(StringFollower(async).follow("'bkptno' rhs"), breakpoint_number);
            if(breakpoint_number != BreakpointSetupObserver::FREE) return false;
            
            /* TODO: fix this StringFollower path . . . */
            /*Misc::String::to<std::size_t>(StringFollower(async).follow("'frame' rhs 'args' rhs [0] 'value' rhs"), block_address, true);*/
            
            Initializer::get_instance()->get_controller()->send_command(
                Misc::StreamAsString() << "-break-disable " << BreakpointSetupObserver::FREE);
            Initializer::get_instance()->get_controller()->send_command("-exec-finish");
        }
        else if(StringFollower(async).follow("'reason' rhs") == "function-finished") {
            Initializer::get_instance()->get_controller()->send_command(
                Misc::StreamAsString() << "-break-enable " << BreakpointSetupObserver::FREE);
            
        }
    }
    return false;
}


} // namespace GDB
} // namespace Interface
} // namespace Aesalon
