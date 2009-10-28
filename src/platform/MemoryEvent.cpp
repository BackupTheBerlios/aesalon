#include <string>

#include "MemoryEvent.h"
#include "misc/StreamAsString.h"

namespace Aesalon {
namespace Platform {

MemoryEvent *MemoryEvent::deserialize(std::string data) {
    return NULL;
}

std::string MemoryBlockAllocEvent::serialize() const {
    return Misc::StreamAsString() << "mbae:" << get_address() << ":" << get_size();
}

std::string MemoryBlockResizedEvent::serialize() const {
    return Misc::StreamAsString() << "mbre:" << get_address() << ":" << get_size();
}

std::string MemoryBlockFreeEvent::serialize() const {
    return Misc::StreamAsString() << "mbfe:" << std::string(*get_block());
}   

std::string MemoryReferenceNewEvent::serialize() const {
    return Misc::StreamAsString() << "mrne:" << std::string(*get_scope()) << ":" << std::string(*get_block());
}

std::string MemoryReferenceChangedEvent::serialize() const {
    return Misc::StreamAsString() << "mrce:" << std::string(*get_scope()) << ":"
        << std::string(*get_block()) << ":" << std::string(*get_new_block());
}

std::string MemoryReferenceRemovedEvent::serialize() const {
    return Misc::StreamAsString() << "mrre:" << std::string(*get_scope()) << ":" << std::string(*get_block());
}

} // namespace Platform
} // namespace Aesalon
