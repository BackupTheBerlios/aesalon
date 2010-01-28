#include "EventQueue.h"
#include <iostream>

namespace Aesalon {
namespace Misc {

void EventQueue::push_event(Misc::SmartPointer<Event> event) {
    if(event.is_valid()) event_queue.push(event);
}

void EventQueue::pop_event() {
    if(event_queue.empty()) return;
    Event *event = event_queue.front();
    event_queue.pop();
    delete event;
}

Misc::SmartPointer<Event> EventQueue::peek_event() {
    if(event_queue.empty()) return NULL;
    return event_queue.front();
}

} // namespace Misc
} // namespace Aesalon

