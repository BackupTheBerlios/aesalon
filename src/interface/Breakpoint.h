#ifndef AESALON_INTERFACE_BREAKPOINT_H
#define AESALON_INTERFACE_BREAKPOINT_H

#include "Types.h"
#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Interface {

class Breakpoint {
private:
    Platform::MemoryAddress address;
    Byte original;
public:
    Breakpoint(Platform::MemoryAddress address, Byte original)
        : address(address), original(original) {}
    virtual ~Breakpoint() {}
    
    Byte get_original() const { return original; }
    Platform::MemoryAddress get_address() const { return address; }
};

} // namespace Interface
} // namespace Aesalon

#endif
