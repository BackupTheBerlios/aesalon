#include "MonitorEvent.h"

namespace Event {

Block* MonitorEvent::serialize() {
    Block *serialized = BasicEvent::serialize();
    serialized->get_data()[0] |= (monitor_event_type << 2) & 0x04;
    return serialized;
}

} // namespace Event
