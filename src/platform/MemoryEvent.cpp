#include <string>

#include "MemoryEvent.h"
#include "misc/StreamAsString.h"

namespace Aesalon {
namespace Platform {

/*MemoryEvent *MemoryEvent::deserialize(std::string data) {
    return NULL;
}*/

std::string MemoryBlockAllocEvent::serialize() const {
    return Misc::StreamAsString() << "mbae:" << get_block_address() << ":" << get_block_size();
}

std::string MemoryBlockResizeEvent::serialize() const {
    return Misc::StreamAsString() << "mbre:" << get_old_block_address()
        << ":" << get_new_block_address() << ":" << get_new_block_size();
}

std::string MemoryBlockFreeEvent::serialize() const {
    return Misc::StreamAsString() << "mbfe:" << get_block_address();
}

} // namespace Platform
} // namespace Aesalon
