#ifndef AESALON_MONITOR_BREAKPOINT_H
#define AESALON_MONITOR_BREAKPOINT_H

#include "Types.h"
#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Monitor {

class Breakpoint {
private:
    Platform::MemoryAddress address;
    Byte original;
    const Byte BREAKPOINT_CHARACTER;
    std::size_t id;
    bool valid;
public:
    Breakpoint(Platform::MemoryAddress address, Byte original);
    virtual ~Breakpoint() {}
    
    std::size_t get_id() const { return id; }
    Byte get_original() const { return original; }
    Platform::MemoryAddress get_address() const { return address; }
    Byte get_breakpoint_character() const { return BREAKPOINT_CHARACTER; }
    
    bool is_valid() const { return valid; }
    void set_valid(bool new_validity) { valid = new_validity; }
};

} // namespace Monitor
} // namespace Aesalon

#endif
