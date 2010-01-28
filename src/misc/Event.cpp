#include "Event.h"
#include "BlockEvent.h"
#include "StreamAsString.h"
#include "Exception.h"

namespace Aesalon {
namespace Misc {

std::string Event::serialize() {
    if(type == BLOCK_EVENT) return Misc::StreamAsString() << 0x01;
    throw Misc::Exception("Asked to serialize invalid Event");
}

Misc::SmartPointer<Event> Event::deserialize(std::string data) {
    if(data[0] == '1') return BlockEvent::deserialize(data.substr(1));
    return NULL;
}

} // namespace Misc
} // namespace Aesalon
