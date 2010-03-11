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
    
    @file storage/Block.h
*/

#ifndef AESALON_GUI_STORAGE_BLOCK_H
#define AESALON_GUI_STORAGE_BLOCK_H

#include "MemoryTypes.h"
#include "Timestamp.h"

class Block {
private:
    Timestamp allocation_time;
    Timestamp release_time;
    MemoryAddress address;
    MemorySize size;
public:
    Block(const Timestamp &allocation_time, MemoryAddress address, MemorySize size) : allocation_time(allocation_time), release_time(0), address(address), size(size) {}
    
    Block *clone();
    
    const Timestamp &get_allocation_time() const { return allocation_time; }
    const Timestamp &get_release_time() const { return release_time; }
    void set_release_time(const Timestamp &time) { release_time = time; }
    MemoryAddress get_address() const { return address; }
    MemorySize get_size() const { return size; }
};

#endif
