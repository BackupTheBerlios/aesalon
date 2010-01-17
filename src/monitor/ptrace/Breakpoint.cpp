#include "Breakpoint.h"
#include "BreakpointReference.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

Breakpoint::Breakpoint(Platform::MemoryAddress address, Byte original)
    : address(address), original(original), 
#if AESALON_PLATFORM == AESALON_PLATFORM_x86
        BREAKPOINT_CHARACTER(0xcc),
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86_64
        BREAKPOINT_CHARACTER(0xcc),
#endif
    valid(true)
{
    static std::size_t last_id = 0;
    id = ++last_id;
    
}

void Breakpoint::notify() {
    for(observer_list_t::iterator i = observer_list.begin(); i != observer_list.end(); i ++) {
        (*i)->handle_breakpoint(BreakpointReference(this));
    }
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
