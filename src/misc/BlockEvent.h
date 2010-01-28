#ifndef AESALON_MISC_BLOCK_EVENT_H
#define AESALON_MISC_BLOCK_EVENT_H

#include "Event.h"
#include "Types.h"

namespace Aesalon {
namespace Misc {

class BlockEvent : public Event {
public:
    enum block_event_type_e {
        ALLOC_EVENT,
        REALLOC_EVENT,
        FREE_EVENT
    };
private:
    block_event_type_e block_type;
    
    uint_64 address, size, new_address;
public:
    BlockEvent(block_event_type_e type, uint_64 address, uint_64 size = 0,
        uint_64 new_address = 0, uint_64 new_size = 0) : Event(BLOCK_EVENT), block_type(type),
        address(address), size(size), new_address(new_address) {}
    virtual ~BlockEvent() {}
    
    block_event_type_e get_block_type() const { return block_type; }
    
    uint_64 get_address() const { return address; }
    uint_64 get_size() const { return size; }
    uint_64 get_new_address() const { return new_address; }
    
    virtual std::string serialize();
    static Misc::SmartPointer<Event> deserialize(std::string data);
};

} // namespace Misc
} // namespace Aesalon

#endif
