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
    
    @file tracker/MemoryBlock.h
*/

#ifndef AESALON_MONITOR_MEMORY_BLOCK_H
#define AESALON_MONITOR_MEMORY_BLOCK_H

#include "Types.h"

namespace Tracker {

/** A representation of a block of allocated memory. */
class MemoryBlock {
private:
    /** The address of the block. */
    Word address;
    /** The size of the block. */
    Word size;
public:
    /** Generic constructor.
        @param address The address of the block.
        @param size The size of the block.
    */
    MemoryBlock(Word address, Word size) : address(address), size(size) {}
    
    /** Gets the address of the current block.
        @return The address of the block.
    */
    Word get_address() const { return address; }
    /** Changes the address of the current block.
        @param new_address The new address.
    */
    void set_address(Word new_address) { address = new_address; }
    /** Returns the current size of the current block.
        @return The current size.
    */
    Word get_size() const { return size; }
    /** Changes the size of the current block.
        @param new_size The new size.
    */
    void set_size(Word new_size) { size = new_size; }
};

} // namespace Tracker

#endif
