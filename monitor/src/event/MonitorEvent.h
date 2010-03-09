#ifndef AESALON_MONITOR_EVENT_H
#define AESALON_MONITOR_EVENT_H

#include "BasicEvent.h"

namespace Event {

class MonitorEvent : public BasicEvent {
public:
    enum monitor_event_type_e {
        PROGRAM_STARTED = 0,
        PROGRAM_FINISHED = 1
    };
private:
    monitor_event_type_e monitor_event_type;
public:
    MonitorEvent(monitor_event_type_e monitor_event_type) : BasicEvent(MONITOR_EVENT), monitor_event_type(monitor_event_type) {}
    virtual ~MonitorEvent() {}
    
    virtual Block* serialize();
};

} // namespace Event

#endif
