#include "BasicEvent.h"
#include "Block.h"
#include "misc/StreamAsString.h"
#include "exception/EventException.h"

namespace Event {

std::string BasicEvent::serialize() {
    if(type == BLOCK_EVENT) return Misc::StreamAsString() << 0x01;
    throw Exception::EventException("Asked to serialize invalid Event");
}

} // namespace Event
