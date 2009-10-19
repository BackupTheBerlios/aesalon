#ifndef AESALON_MISC_EVENT_QUEUE_H
#define AESALON_MISC_EVENT_QUEUE_H

#include <iostream>
#include <queue>

#include "Event.h"
#include "Singleton.h"

namespace Aesalon {
namespace Misc {

/** Event queue singleton class. Wrapper around std::queue. */
class EventQueue : public Singleton<EventQueue> {
public:
    typedef std::queue<Event *> event_queue_t;
private:
    event_queue_t event_queue;
public:
    /** Basic constructor, calls Singleton constructor. */
    EventQueue() : Singleton<EventQueue>() {}
    /** Destructor, does nothing. */
    virtual ~EventQueue() {}
    
    /** Add another event onto the FIFO queue.
        @param event The event to push on the queue.
    */
    void push_event(Event *event);
    /** Remove an event from the queue, and delete its instance. */
    void pop_event();
    /** Get the first element on the queue.
        @return The first element, or NULL if there are no elements.
    */
    Event *peek_event();
};

} // namespace Misc
} // namespace Aesalon

#endif
