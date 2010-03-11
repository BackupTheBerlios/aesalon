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
    
    @file event/BasicEvent.h
*/

#ifndef AESALON_MISC_EVENT_H
#define AESALON_MISC_EVENT_H

#include <string>

#include "Types.h"

namespace Event {

/** Event class. Note that until overloaded, this base class is rather
    useless. */
class BasicEvent {
public:
    /** Event type enum, used for dynamic_cast<>ing later. */
    enum event_type_e {
        BLOCK_EVENT,
        MONITOR_EVENT
    };
private:
    /** Current event type. */
    event_type_e type;
    u_int64_t timestamp;
public:
    /** Constructor, takes a type. All other stored data is in the derived
        types. */
    BasicEvent(event_type_e type);
    virtual ~BasicEvent() {}
    
    /** Returns the type of the current event.
        @return The type of the current event.
    */
    event_type_e get_type() const { return type; }
    u_int64_t get_timestamp() const { return timestamp; }
    
    virtual Block *serialize();
};

} // namespace Event

#endif
