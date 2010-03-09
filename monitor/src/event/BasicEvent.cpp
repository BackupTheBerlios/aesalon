#include <time.h>
#include "BasicEvent.h"
#include "BlockEvent.h"
#include "misc/StreamAsString.h"
#include "exception/EventException.h"

namespace Event {

BasicEvent::BasicEvent(BasicEvent::event_type_e type) : type(type) {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    timestamp = time.tv_sec * 1000000000;
    timestamp += time.tv_nsec;
}


/* Serialization format:
    first bit defines the event type, BLOCK_EVENT or REFERENCE_EVENT
*/
Block *BasicEvent::serialize() {
    Block *block = new Block();
    block->resize(1);
    if(type == BLOCK_EVENT) {
        block->get_data()[0] |= 0x01;
    }
    else if(type == MONITOR_EVENT) {
        block->get_data()[0] |= 0x02;
    }
    else throw Exception::EventException("Asked to serialize invalid Event");
    block->push_word(timestamp);
    return block;
}

} // namespace Event
