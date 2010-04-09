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
    
    @file event/BasicEvent.cpp
*/

#include <time.h>
#include "BasicEvent.h"
#include "BlockEvent.h"
#include "misc/StreamAsString.h"
#include "exception/EventException.h"

namespace Event {

BasicEvent::BasicEvent(BasicEvent::event_type_e type) : type(type) {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    timestamp = time.tv_sec * 1000000000;
    timestamp += time.tv_nsec;
}

/* Serialization format:
    first bit defines the event type, BLOCK_EVENT or REFERENCE_EVENT
*/
Block *BasicEvent::serialize() {
    Block *block = new Block();
    block->resize(1);
    block->get_data()[0] = 0x0;
    if(type == BLOCK_EVENT) {
        block->get_data()[0] |= 0x01;
    }
    else if(type == MONITOR_EVENT) {
        block->get_data()[0] |= 0x02;
    }
    else if(type == SCOPE_EVENT) {
        block->get_data()[0] |= 0x03;
    }
    else throw Exception::EventException("Asked to serialize invalid Event");
    block->push_word(timestamp);
    return block;
}

} // namespace Event
