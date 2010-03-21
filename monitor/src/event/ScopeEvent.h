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
    
    @file event/ScopeEvent.h
*/
#ifndef AESALON_EVENT_SCOPE_H
#define AESALON_EVENT_SCOPE_H

#include <string>

#include "BasicEvent.h"

namespace Event {

class ScopeEvent : public BasicEvent {
public:
    enum scope_event_type_e {
        NEW_SCOPE
    };
private:
    scope_event_type_e scope_type;
    Word scope_id;
    std::string name;
public:
    ScopeEvent(scope_event_type_e scope_type, Word id, std::string name);
    virtual ~ScopeEvent();
    
    Word get_scope_id() const { return scope_id; }
    std::string get_name() const { return name; }
    
    virtual Block* serialize();
};

} // namespace Event

#endif
