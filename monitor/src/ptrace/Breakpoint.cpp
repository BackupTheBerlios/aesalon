#include "Breakpoint.h"
#include "BreakpointReference.h"

namespace Aesalon {
namespace Monitor {
namespace PTrace {

Breakpoint::Breakpoint(Word address, Byte original)
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
    if(observer_list.size() == 0) {
        set_valid(false);
        return;
    }
    int last_size = observer_list.size();
    for(int x = 0; x < last_size && observer_list.size(); x ++) {
        observer_list_t::iterator i = observer_list.begin();
        for(int y = 0; y < x; y ++) i ++;
        if((*i).is_valid()) (*i)->handle_breakpoint(BreakpointReference(this));
        if((unsigned)last_size != observer_list.size()) x --;
    }
}

} // namespace PTrace
} // namespace Monitor
} // namespace Aesalon
