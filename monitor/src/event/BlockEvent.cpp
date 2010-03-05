#include <iostream>
#include "BlockEvent.h"
#include "exception/EventException.h"
#include "misc/StreamAsString.h"

namespace Event {

/* serialization format:
    first bit is taken by BasicEvent
    second and third bits are block_type
    rest of the first byte is reserved
    next eight bytes are timestamp
    after that, depends on type . . .
*/
Block *BlockEvent::serialize() {
    Block *serialized = BasicEvent::serialize();
    
    serialized->get_data()[0] |= (block_type << 1) & 0x06;
    
    serialized->push_word(get_address());

    switch(get_block_type()) {
        case ALLOC_EVENT:
            serialized->push_word(get_size());
            break;
        case REALLOC_EVENT:
            /* NOTE: the new address first makes it easier to deserialize later */
            serialized->push_word(get_new_address());
            serialized->push_word(get_size());
            break;
        case FREE_EVENT: break;
        default:
            throw Exception::EventException("Asked to serialize invalid Block event");
    }
    
    return serialized;
}

} // namespace Event
