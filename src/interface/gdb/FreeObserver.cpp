#include "FreeObserver.h"
#include "StringFollower.h"
#include "interface/Initializer.h"
#include "misc/String.h"
#include "BreakpointSetupObserver.h"
#include "platform/MemoryEvent.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool FreeObserver::notify(Misc::SmartPointer<AsyncOutput> async) {
    if(async->get_data()->get_first() == "stopped") {
        if(StringFollower(async).follow("'reason' rhs") == "breakpoint-hit") {
            int breakpoint_number;
            Misc::String::to<int>(StringFollower(async).follow("'bkptno' rhs"), breakpoint_number);
            if(breakpoint_number != BreakpointSetupObserver::FREE) return false;
            
            Misc::String::to<std::size_t>(StringFollower(async).follow("'frame' rhs 'args' rhs [0] 'value' rhs"), block_address, true);
            
            Initializer::get_instance()->get_controller()->send_command(
                Misc::StreamAsString() << "-break-disable " << BreakpointSetupObserver::FREE);
            Initializer::get_instance()->get_controller()->send_command("-exec-finish");
            waiting = true;
        }
        else if(StringFollower(async).follow("'reason' rhs") == "function-finished" && waiting) {
            Initializer::get_instance()->get_controller()->send_command(
                Misc::StreamAsString() << "-break-enable " << BreakpointSetupObserver::FREE);
            
            
            event_queue->push_event(new Platform::MemoryBlockFreeEvent(block_address));
            
            waiting = false;
        }
    }
    return false;
}


} // namespace GDB
} // namespace Interface
} // namespace Aesalon
