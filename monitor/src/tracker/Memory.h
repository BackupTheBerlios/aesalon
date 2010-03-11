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
    
    @file tracker/Memory.h
*/

#ifndef AESALON_PLATFORM_MEMORY_H
#define AESALON_PLATFORM_MEMORY_H

#include <string>
#include <set>

#include "MemoryBlock.h"
#include "event/BasicEvent.h"
#include "Types.h"

namespace Tracker {

/** Stores the memory associated with a program. */
class Memory {
protected:
    typedef std::set<MemoryBlock *> block_set_t;
private:
    block_set_t block_set;
    
    void add_block(MemoryBlock *new_block) {
        block_set.insert(new_block);
    }
    void remove_block(MemoryBlock *block);
public:
    /** Generic constructor. */
    Memory() {}
    /** Virtual destructor. */
    virtual ~Memory() {}
    
    /** Returns the block at the specified MemoryAddress.
        @param address The address to look for.
        @return The @a MemoryBlock at @a address, or NULL if not found.
    */
    MemoryBlock *get_block(Word address) const;
    
    /** Handle an event as a "diff", and apply it to the current memory representation.
        @param event The event to handle.
    */
    void handle_event(Event::BasicEvent *event);
};

} // namespace Tracker

#endif
