#ifndef AESALON_MISC_BLOCK_EVENT_H
#define AESALON_MISC_BLOCK_EVENT_H

#include "BasicEvent.h"
#include "Types.h"

namespace Event {

class BlockEvent : public BasicEvent {
public:
    enum block_event_type_e {
        ALLOC_EVENT,
        REALLOC_EVENT,
        FREE_EVENT
    };
private:
    block_event_type_e block_type;
    
    Word address, size, new_address;
public:
    BlockEvent(block_event_type_e type, Word address, Word size = 0,
        Word new_address = 0, Word new_size = 0) : BasicEvent(BLOCK_EVENT), block_type(type),
        address(address), size(size), new_address(new_address) {}
    virtual ~BlockEvent() {}
    
    block_event_type_e get_block_type() const { return block_type; }
    
    Word get_address() const { return address; }
    Word get_size() const { return size; }
    Word get_new_address() const { return new_address; }
    
    virtual Block *serialize();
};

} // namesace Event

#endif
