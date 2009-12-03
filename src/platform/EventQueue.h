#ifndef AESALON_PLATFORM_EVENT_QUEUE_H
#define AESALON_PLATFORM_EVENT_QUEUE_H

#include <iostream>
#include <queue>

#include "Event.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Platform {

/** Event queue class. Wrapper around std::queue. */
class EventQueue {
public:
    typedef std::queue<Misc::SmartPointer<Event> > event_queue_t;
private:
    event_queue_t event_queue;
public:
    /** Basic constructor, calls Singleton constructor. */
    EventQueue() {}
    /** Destructor, does nothing. */
    virtual ~EventQueue() {}
    
    /** Add another event onto the FIFO queue.
        @param event The event to push on the queue.
    */
    void push_event(Misc::SmartPointer<Event> event);
    /** Remove an event from the queue, and delete its instance. */
    void pop_event();
    /** Get the first element on the queue.
        @return The first element, or NULL if there are no elements.
    */
    Misc::SmartPointer<Event> peek_event();
};

} // namespace Platform
} // namespace Aesalon

#endif
