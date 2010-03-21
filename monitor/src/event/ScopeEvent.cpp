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
    
    @file event/ScopeEvent.cpp
*/
#include "ScopeEvent.h"
#include "misc/Message.h"
#include <cstring>

namespace Event {

ScopeEvent::ScopeEvent(scope_event_type_e scope_type, Word id, std::string name)
    : BasicEvent(BasicEvent::SCOPE_EVENT), scope_type(scope_type), scope_id(id), name(name) {

}

ScopeEvent::~ScopeEvent() {

}

Block* ScopeEvent::serialize() {
    Block *serialized = BasicEvent::serialize();
    
    const char *data = name.c_str();
    std::size_t length = name.length();
    if(length > 256) {
        Misc::Message(Misc::Message::WARNING_MESSAGE, "scope name length > 256 characters, truncating");
        name.erase(255);
        data = name.c_str();
    }
    
    std::size_t pos = serialized->get_size();
    serialized->resize(pos + length + 1);
    serialized->get_data()[pos] = length & 0xff;
    memcpy(serialized->get_data()+pos+1, data, length % 0x100);
    
    return serialized;
}


} // namespace Event
