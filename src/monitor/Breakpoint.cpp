#include "Breakpoint.h"

namespace Aesalon {
namespace Monitor {

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

} // namespace Monitor
} // namespace Aesalon
