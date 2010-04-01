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
    
    @file storage/Block.cpp
*/

#include "Block.h"

Block::Block(const Timestamp &allocation_time, MemoryAddress address, MemorySize size, MemoryAddress alloc_scope)
    : allocation_time(allocation_time), release_time(0), address(address), size(size), allocation_scope(alloc_scope),
    release_scope(0) {

}


Block* Block::clone() {
    Block *block = new Block(get_allocation_time(), address, size, get_allocation_scope());
    block->set_release_time(get_release_time());
    block->release_scope = release_scope;
    return block;
}
