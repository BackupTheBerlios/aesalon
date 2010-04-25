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
    
    @file event/BlockEvent.h
*/

#ifndef AESALON_MISC_BLOCK_EVENT_H
#define AESALON_MISC_BLOCK_EVENT_H

#include "BasicEvent.h"
#include "Types.h"

namespace Event {

class BlockEvent : public BasicEvent {
public:
    enum block_event_type_e {
        ALLOC_EVENT,
        REALLOC_EVENT,
        FREE_EVENT
    };
private:
    block_event_type_e block_type;
    
    Word *scope;
    Word scope_size;
    Word address, size, new_address;
public:
    BlockEvent(block_event_type_e type, Word64 timestamp, Word address, Word size = 0,
        Word new_address = 0, Word new_size = 0) : BasicEvent(BLOCK_EVENT, timestamp), block_type(type),
        scope(NULL), scope_size(0), address(address), size(size), new_address(new_address) {}
    virtual ~BlockEvent();
    
    block_event_type_e get_block_type() const { return block_type; }
    
    Word *get_scope() const { return scope; }
    void set_scope(Word *new_scope) { scope = new_scope; }
    
    Word get_scope_size() const { return scope_size; }
    void set_scope_size(Word new_size) { scope_size = new_size; }
    
    Word get_address() const { return address; }
    Word get_size() const { return size; }
    Word get_new_address() const { return new_address; }
    
    virtual Block *serialize(int bits);
};

} // namesace Event

#endif
