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
    
    @file event/MonitorEvent.h
*/

#ifndef AESALON_MONITOR_EVENT_H
#define AESALON_MONITOR_EVENT_H

#include "BasicEvent.h"

namespace Event {

class MonitorEvent : public BasicEvent {
public:
    enum monitor_event_type_e {
        PROGRAM_STARTED = 0,
        PROGRAM_FINISHED = 1
    };
private:
    monitor_event_type_e monitor_event_type;
public:
    MonitorEvent(monitor_event_type_e monitor_event_type) : BasicEvent(MONITOR_EVENT), monitor_event_type(monitor_event_type) {}
    virtual ~MonitorEvent() {}
    
    virtual Block* serialize();
};

} // namespace Event

#endif
