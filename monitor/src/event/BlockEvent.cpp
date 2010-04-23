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
    
    @file event/BlockEvent.cpp
*/

#include <iostream>
#include "BlockEvent.h"
#include "exception/EventException.h"
#include "misc/StreamAsString.h"
#include "Initializer.h"
#include "ScopeEvent.h"

namespace Event {

/* serialization format:
    first two bits are taken by BasicEvent
    third and fourth bits are block_type
    rest of the first byte is reserved
    next eight bytes are timestamp
    after that, depends on type . . .
*/
Block *BlockEvent::serialize(int bits) {
    Block *serialized = BasicEvent::serialize(bits);
    Word32 scope_id;
    ScopeEvent *event = Initializer::get_instance()->get_scope_manager()->get_scope(scope, scope_id);
    if(event) {
        Block *block = event->serialize(bits);
        serialized->prepend(block);
        delete block;
        delete event;
    }
    
    serialized->get_data()[0] |= (block_type << 2) & 0x0c;
    
    serialized->push_word(scope_id, 32);
    serialized->push_word(address, bits);

    switch(get_block_type()) {
        case ALLOC_EVENT:
            serialized->push_word(size, bits);
            break;
        case REALLOC_EVENT:
            /* NOTE: the new address first makes it easier to deserialize later */
            serialized->push_word(new_address, bits);
            serialized->push_word(size, bits);
            break;
        case FREE_EVENT: break;
        default:
            throw Exception::EventException("Asked to serialize invalid Block event");
    }
    
    return serialized;
}

} // namespace Event
