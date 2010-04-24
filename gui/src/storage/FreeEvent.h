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
    
    @file storage/FreeEvent.h
*/

#ifndef AESALON_GUI_STORAGE_FREE_EVENT_H
#define AESALON_GUI_STORAGE_FREE_EVENT_H

#include "Event.h"
#include "MemoryTypes.h"
#include "storage/Scope.h"

class FreeEvent : public Event {
private:
    MemoryAddress address;
    Backtrace scope;
public:
    FreeEvent(const Timestamp &timestamp, MemoryAddress address, const Backtrace &scope) : Event(timestamp), address(address), scope(scope) {}
    virtual ~FreeEvent() {}
    
    MemoryAddress get_address() const { return address; }
    const Backtrace &get_scope() const { return scope; }
    
    virtual void apply_to(Snapshot *snapshot);  
    
    virtual void accept(EventVisitor &visitor);
};

#endif
