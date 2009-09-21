#ifndef AESALON_INTERFACE_EVENT_QUEUE_H
#define AESALON_INTERFACE_EVENT_QUEUE_H

#include <queue>

#include "Event.h"
#include "misc/Singleton.h"

namespace Aesalon {
namespace Interface {

class EventQueue : public Misc::Singleton<EventQueue> {
public:
    typedef std::queue<Event *> event_queue_t;
private:
    event_queue_t event_queue;
public:
    EventQueue() : Misc::Singleton<EventQueue>() {}
    
    void push_event(Event *event);
    void pop_event();
    Event *peek_event();
};

} // namespace Interface
} // namespace Aesalon

#endif
