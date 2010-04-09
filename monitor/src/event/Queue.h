/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file event/Queue.h
*/

#ifndef AESALON_MISC_EVENT_QUEUE_H
#define AESALON_MISC_EVENT_QUEUE_H

#include <iostream>
#include <queue>

#include <pthread.h>

#include "BasicEvent.h"

namespace Event {

/** Event queue class. Mutex wrapper around std::queue. */
class Queue {
public:
    typedef std::queue<BasicEvent *> event_queue_t;
private:
    pthread_mutex_t mutex;
    event_queue_t event_queue;
public:
    Queue();
    virtual ~Queue();
    
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
    
    void lock_mutex();
    void unlock_mutex();
};

} // namespace Event

#endif
