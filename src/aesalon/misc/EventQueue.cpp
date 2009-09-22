#include "EventQueue.h"
#include <iostream>

namespace Aesalon {
namespace Misc {

template<>EventQueue *Singleton<EventQueue>::instance = 0;

void EventQueue::push_event(Event *event) {
    if(event == NULL) return;
    std::cout << "Adding event to queue:" << event << std::endl;
    std::cout << "\tFrom EventQueue address: " << get_instance() << std::endl;
    std::cout << "\tCurrently, empty is: " << event_queue.empty() << std::endl;
    event_queue.push(event);
    std::cout << "\tFront element: " << peek_event() << std::endl;
}

void EventQueue::pop_event() {
    if(event_queue.empty()) return;
    Event *event = event_queue.front();
    event_queue.pop();
    delete event;
}

Event *EventQueue::peek_event() {
    std::cout << "Peeking event: " << std::endl;
    std::cout << "\t" << event_queue.front() << std::endl;
    std::cout << "\tEventQueue address:" << get_instance() << std::endl;
    if(event_queue.empty()) return NULL;
    return event_queue.front();
}

} // namespace Misc
} // namespace Aesalon