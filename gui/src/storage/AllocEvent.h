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
    
    @file storage/AllocEvent.h
*/

#ifndef AESALON_GUI_STORAGE_ALLOC_EVENT_H
#define AESALON_GUI_STORAGE_ALLOC_EVENT_H

#include "Event.h"
#include "MemoryTypes.h"

class AllocEvent : public Event {
private:
    MemoryAddress address;
    MemorySize size;
    const Scope &scope;
public:
    AllocEvent(const Timestamp &timestamp, MemoryAddress address, MemorySize size, const Scope &scope)
        : Event(timestamp), address(address), size(size), scope(scope) {}
    virtual ~AllocEvent() {}
    
    MemoryAddress get_address() const { return address; }
    MemorySize get_size() const { return size; }
    const Scope &get_scope() const { return scope; }
    
    virtual void apply_to(Snapshot *snapshot);
    
    virtual void accept(EventVisitor &visitor);
};

#endif
