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
    
    @file event/Queue.cpp
*/

#include "Queue.h"
#include <iostream>

namespace Event {

Queue::Queue() {
    pthread_mutex_init(&mutex, NULL);
}

Queue::~Queue() {
    pthread_mutex_destroy(&mutex);
}

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

void Event::Queue::lock_mutex() {
    pthread_mutex_lock(&mutex);
}

void Event::Queue::unlock_mutex() {
    pthread_mutex_unlock(&mutex);
}


}
