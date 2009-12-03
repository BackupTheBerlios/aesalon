#include "MallocObserver.h"
#include "StringFollower.h"
#include "interface/Initializer.h"
#include "misc/String.h"
#include "BreakpointSetupObserver.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool MallocObserver::notify(Misc::SmartPointer<AsyncOutput> async) {
    if(async->get_data()->get_first() == "stopped") {
        if(StringFollower(async).follow("'reason' rhs") == "breakpoint-hit") {
            int breakpoint_number;
            Misc::String::to<int>(StringFollower(async).follow("'bkptno' rhs"), breakpoint_number);
            if(breakpoint_number != BreakpointSetupObserver::MALLOC) return false;
            
            /* Example line:
*stopped,reason="breakpoint-hit",disp="keep",bkptno="2",frame={addr="<address>",func="*__GI__libc_malloc",args=[{name="bytes,value="1"}],file="malloc.c"}
                
                Therefore, "'frame' rhs args rhs [0] value rhs" . . .
            */
            
            Misc::String::to<std::size_t>(StringFollower(async).follow("'frame' rhs 'args' rhs [0] 'value' rhs"), last_size);
            last_size = 1; 
            
            Initializer::get_instance()->get_controller()->send_command(
                Misc::StreamAsString() << "-break-disable " << BreakpointSetupObserver::MALLOC);
            Initializer::get_instance()->get_controller()->send_command("-exec-finish");
        }
        else if(StringFollower(async).follow("'reason' rhs") == "function-finished") {
            Initializer::get_instance()->get_controller()->send_command(
                Misc::StreamAsString() << "-break-enable " << BreakpointSetupObserver::MALLOC);
            
        }
    }
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
