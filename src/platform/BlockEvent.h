#ifndef AESALON_PLATFORM_BLOCK_EVENT_H
#define AESALON_PLATFORM_BLOCK_EVENT_H

#include "Event.h"
#include "MemoryAddress.h"

namespace Aesalon {
namespace Platform {

class BlockEvent : public Event {
public:
    enum block_event_type_e {
        ALLOC_EVENT,
        REALLOC_EVENT,
        FREE_EVENT
    };
private:
    block_event_type_e block_type;
    
    MemoryAddress address, size, new_address;
public:
    BlockEvent(block_event_type_e type, MemoryAddress address, MemoryAddress size = 0,
        MemoryAddress new_address = 0, MemoryAddress new_size = 0) : Event(BLOCK_EVENT), block_type(type),
        address(address), size(size), new_address(new_address) {}
    virtual ~BlockEvent() {}
    
    block_event_type_e get_block_type() const { return block_type; }
    
    MemoryAddress get_address() const { return address; }
    MemoryAddress get_size() const { return size; }
    MemoryAddress get_new_address() const { return new_address; }
    
    virtual std::string serialize();
    static Misc::SmartPointer<Event> deserialize(std::string data);
};

} // namespace Platform
} // namespace Aesalon

#endif
