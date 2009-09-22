#ifndef AESALON_MISC_EVENT_QUEUE_H
#define AESALON_MISC_EVENT_QUEUE_H

#include <iostream>
#include <queue>

#include "Event.h"
#include "Singleton.h"

namespace Aesalon {
namespace Misc {

class EventQueue : public Singleton<EventQueue> {
public:
    typedef std::queue<Event *> event_queue_t;
private:
    event_queue_t event_queue;
public:
    EventQueue() : Singleton<EventQueue>() {
        std::cout << "Constructing EventQueue" << std::endl;
    }
    virtual ~EventQueue() {
        std::cout << "Destructing EventQueue" << std::endl;
        std::cout << event_queue.empty() << std::endl;
    }
    
    void push_event(Event *event);
    void pop_event();
    Event *peek_event();
};

} // namespace Misc
} // namespace Aesalon

#endif
