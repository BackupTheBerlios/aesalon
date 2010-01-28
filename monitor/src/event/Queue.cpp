#include "Queue.h"
#include <iostream>

namespace Event {

void Queue::push_event(BasicEvent *event) {
    if(event) event_queue.push(event);
}

void Queue::pop_event() {
    if(event_queue.empty()) return;
    BasicEvent *event = event_queue.front();
    event_queue.pop();
    delete event;
}

BasicEvent *Queue::peek_event() {
    if(event_queue.empty()) return NULL;
    return event_queue.front();
}

}