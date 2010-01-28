#ifndef AESALON_MISC_EVENT_QUEUE_H
#define AESALON_MISC_EVENT_QUEUE_H

#include <iostream>
#include <queue>

#include "BasicEvent.h"

namespace Event {

/** Event queue class. Wrapper around std::queue. */
class Queue {
public:
    typedef std::queue<BasicEvent *> event_queue_t;
private:
    event_queue_t event_queue;
public:
    /** Basic constructor, does nothing. */
    Queue() {}
    /** Destructor, also does nothing. */
    virtual ~Queue() {}
    
    /** Add another event onto the FIFO queue.
        @param event The event to push on the queue.
    */
    void push_event(BasicEvent *event);
    /** Remove an event from the queue, and delete its instance. */
    void pop_event();
    /** Get the first element on the queue.
        @return The first element, or NULL if there are no elements.
    */
    BasicEvent *peek_event();
};

} // namespace Event

#endif
