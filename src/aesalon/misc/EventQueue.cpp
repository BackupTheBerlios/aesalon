#include "EventQueue.h"

namespace Aesalon {
namespace Misc {

template<>EventQueue *Singleton<EventQueue>::instance = 0;

void EventQueue::push_event(Event *event) {
    if(event == NULL) return;
    event_queue.push(event);
}

void EventQueue::pop_event() {
    if(event_queue.empty()) return;
    Event *event = event_queue.front();
    event_queue.pop();
    delete event;
}

Event *EventQueue::peek_event() {
    if(event_queue.empty()) return NULL;
    return event_queue.front();
}

} // namespace Misc
} // namespace Aesalon