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
    
    @file tracker/Memory.cpp
*/

#include "Memory.h"
#include "event/BlockEvent.h"

namespace Tracker {

MemoryBlock *Memory::get_block(Word address) const {
    for(block_set_t::const_iterator i = block_set.begin(); i != block_set.end(); i ++) {
        if((*i)->get_address() == address) return *i;
    }
    return NULL;
}

void Memory::remove_block(MemoryBlock *block) {
    for(block_set_t::iterator i = block_set.begin(); i != block_set.end(); i ++) {
        if((*i) != block) continue;
        block_set.erase(i);
        break;
    }
}

void Memory::handle_event(Event::BasicEvent *event) {
    if(event->get_type() == Event::BasicEvent::BLOCK_EVENT) {
        Event::BlockEvent *be = dynamic_cast<Event::BlockEvent *>(event);
        switch(be->get_block_type()) {
            case Event::BlockEvent::ALLOC_EVENT:
                add_block(new MemoryBlock(be->get_address(), be->get_size()));
                break;
            case Event::BlockEvent::FREE_EVENT:
                remove_block(get_block(be->get_address()));
                break;
            case Event::BlockEvent::REALLOC_EVENT:
                MemoryBlock *block = get_block(be->get_address());
                block->set_address(be->get_new_address());
                block->set_size(be->get_size());
                break;
        }
    }
}

} // namespace Tracker
