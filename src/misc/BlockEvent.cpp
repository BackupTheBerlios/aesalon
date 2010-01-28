#include <sstream>
#include "BlockEvent.h"
#include "Exception.h"
#include "StreamAsString.h"
#include "String.h"

namespace Aesalon {
namespace Misc {

std::string BlockEvent::serialize() {
    std::string serialized = Event::serialize();
    
    switch(block_type) {
        case ALLOC_EVENT:
            return Misc::StreamAsString() << Event::serialize() << std::hex << get_address() << ":" << get_size();
        case REALLOC_EVENT:
            return Misc::StreamAsString() << Event::serialize() << std::hex << get_address() << ":" << get_size() << ":" << get_new_address();
        case FREE_EVENT:
            return Misc::StreamAsString() << Event::serialize() << std::hex << get_address();
    }
    throw Misc::Exception("Asked to serialize invalid BlockEvent");
}

Misc::SmartPointer<Event> BlockEvent::deserialize(std::string data) {
    uint_64 address;
    Misc::String::to<uint_64>(data, address, true);
    /* If there's no colon, then it's a FREE_EVENT . . . */
    if(data.find(":") == std::string::npos) {
        return new BlockEvent(FREE_EVENT, address);
    }
    data.erase(0, data.find(":")+1);
    
    uint_64 size;
    Misc::String::to<uint_64>(data, size, true);
    if(data.find(":") == std::string::npos) {
        return new BlockEvent(ALLOC_EVENT, address, size);
    }
    
    data.erase(0, data.find(":")+1);
    uint_64 new_address;
    Misc::String::to<uint_64>(data, new_address, true);
    
    return new BlockEvent(REALLOC_EVENT, address, size, new_address);
}

} // namespace Misc
} // namespace Aesalon
