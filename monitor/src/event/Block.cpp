#include "Block.h"
#include "exception/EventException.h"
#include "misc/StreamAsString.h"


namespace Event {

std::string Block::serialize() {
    std::string serialized = BasicEvent::serialize();
    
    switch(block_type) {
        case ALLOC_EVENT:
            return Misc::StreamAsString() << serialized << std::hex << get_address() << ":" << get_size();
        case REALLOC_EVENT:
            return Misc::StreamAsString() << serialized << std::hex << get_address() << ":" << get_size() << ":" << get_new_address();
        case FREE_EVENT:
            return Misc::StreamAsString() << serialized << std::hex << get_address();
    }
    throw Exception::EventException("Asked to serialize invalid Block event");
}

} // namespace Event