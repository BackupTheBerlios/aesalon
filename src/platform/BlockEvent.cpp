#include <sstream>
#include <iostream>
#include "BlockEvent.h"
#include "misc/Exception.h"
#include "misc/StreamAsString.h"
#include "misc/String.h"

namespace Aesalon {
namespace Platform {

std::string BlockEvent::serialize() {
    std::string serialized = Event::serialize();
    
    switch(block_type) {
        case ALLOC_EVENT:
            return Misc::StreamAsString() << Event::serialize() << std::hex << get_address() << ":" << get_size();
        case REALLOC_EVENT:
            std::cout << "* REALLOC_EVENT serialization:" << std::endl;
            std::cout << "\taddress: " << get_address() << std::endl;
            std::cout << "\tsize: " << get_size() << std::endl;
            std::cout << "\tnew_Address: " << get_new_address() << std::endl;
            return Misc::StreamAsString() << Event::serialize() << std::hex << get_address() << ":" << get_size() << ":" << get_new_address();
        case FREE_EVENT:
            return Misc::StreamAsString() << Event::serialize() << std::hex << get_address();
    }
    throw Misc::Exception("Asked to serialize invalid BlockEvent");
}

Misc::SmartPointer<Event> BlockEvent::deserialize(std::string data) {
    MemoryAddress address;
    Misc::String::to<MemoryAddress>(data, address, true);
    /* If there's no colon, then it's a FREE_EVENT . . . */
    if(data.find(":") == std::string::npos) {
        return new BlockEvent(FREE_EVENT, address);
    }
    data.erase(0, data.find(":")+1);
    
    MemoryAddress size;
    Misc::String::to<MemoryAddress>(data, size, true);
    if(data.find(":") == std::string::npos) {
        return new BlockEvent(ALLOC_EVENT, address, size);
    }
    
    data.erase(0, data.find(":")+1);
    MemoryAddress new_address;
    Misc::String::to<MemoryAddress>(data, new_address, true);
    
    return new BlockEvent(REALLOC_EVENT, address, size, new_address);
}

} // namespace Platform
} // namespace Aesalon
