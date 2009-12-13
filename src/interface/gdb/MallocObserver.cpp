#include <iostream>

#include "MallocObserver.h"
#include "StringFollower.h"
#include "interface/Initializer.h"
#include "misc/String.h"
#include "BreakpointSetupObserver.h"
#include "platform/MemoryEvent.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

bool MallocObserver::notify(Misc::SmartPointer<AsyncOutput> async) {
    std::cout << "MallocObserver::notify(): waiting is: " << waiting << std::endl;
    if(async->get_data()->get_first() == "stopped") {
        if(StringFollower(async).follow("'reason' rhs") == "breakpoint-hit" && !waiting) {
            int breakpoint_number;
            Misc::String::to<int>(StringFollower(async).follow("'bkptno' rhs"), breakpoint_number);
            if(breakpoint_number != BreakpointSetupObserver::MALLOC) return false;
            
            /* Example line:
*stopped,reason="breakpoint-hit",disp="keep",bkptno="2",frame={addr="<address>",func="*__GI__libc_malloc",args=[{name="bytes,value="1"}],file="malloc.c"}
                
                Therefore, "'frame' rhs args rhs [0] value rhs" . . .
            */
            
            Misc::String::to<std::size_t>(StringFollower(async).follow("'frame' rhs 'args' rhs [0] 'value' rhs"), last_size);
            
            Initializer::get_instance()->get_controller()->send_command(
                Misc::StreamAsString() << "-break-disable " << BreakpointSetupObserver::MALLOC);
            Initializer::get_instance()->get_controller()->send_command("-exec-finish");
            waiting = true;
            return true;
        }
        else if(waiting) {
            /*
Normal:
*stopped,reason="breakpoint-hit",disp="keep",bkptno="10",frame={addr="0x4005a2",func="main",args=[]},thread-id="1",stopped-threads="all"
Special:
*stopped,reason="breakpoint-hit",disp="keep",bkptno="11",frame={addr="0x04005bb",func="main",args=[]},gdb-result-var="$1",return-value="(void *) 0x601010",thread-id="1",stopped-threads="all"
So, look to see if "'return-value' rhs" is a string or not . . .
            */
            if(async->get_data()->get_element("return-value").is_valid()) {
                Initializer::get_instance()->get_controller()->send_command(
                Misc::StreamAsString() << "-break-enable " << BreakpointSetupObserver::MALLOC);
            
                Platform::MemoryAddress address;
                Misc::String::to<Platform::MemoryAddress>(StringFollower(async).follow("'return-value' rhs").substr(std::string("(void *)").length()), address, true);
            
                event_queue->push_event(new Platform::MemoryBlockAllocEvent(address, last_size));
                waiting = false;
                Initializer::get_instance()->get_controller()->send_command(Misc::StreamAsString() << "-exec-continue");
                return true;
            }
        }
    }
    return false;
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon
