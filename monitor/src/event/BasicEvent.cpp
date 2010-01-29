#include "BasicEvent.h"
#include "BlockEvent.h"
#include "misc/StreamAsString.h"
#include "exception/EventException.h"

namespace Event {


/* Serialization format:
    first bit defines the event type, BLOCK_EVENT or REFERENCE_EVENT
*/
Block *BasicEvent::serialize() {
    Block *block = new Block();
    block->resize(1);
    if(type == BLOCK_EVENT) {
        block->get_data()[0] |= 0x01;
    }
    else throw Exception::EventException("Asked to serialize invalid Event");
    return block;
}

} // namespace Event
