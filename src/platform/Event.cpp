#include "Event.h"
#include "BlockEvent.h"
#include "misc/StreamAsString.h"
#include "misc/Exception.h"

namespace Aesalon {
namespace Platform {

std::string Event::serialize() {
    if(type == BLOCK_EVENT) return Misc::StreamAsString() << 0x01;
    throw Misc::Exception("Asked to serialize invalid Event");
}

Misc::SmartPointer<Event> Event::deserialize(std::string data) {
    if(data[0] == '1') return BlockEvent::deserialize(data.substr(1));
    return NULL;
}

} // namespace Platform
} // namespace Aesalon
