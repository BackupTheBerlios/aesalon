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
    
    @file Types.cpp
    This file contains the implementation of the classes defined in @a Types.h.
*/

#include <iostream>
#include <cstring>
#include <cstdlib>

#include "Types.h"
#include "exception/OutOfMemoryException.h"

Block::Block(Byte *data, std::size_t data_size) {
    resize(data_size);
    std::memcpy(this->data, data, data_size);
    this->data_size = data_size;
}

void Block::remove(std::size_t from, std::size_t to) {
    if(from > data_size || to > data_size || from >= to) return;
    std::memmove(data + from, data + to, to - from);
}

Block *Block::subset(std::size_t from, std::size_t to) const {
    if(from > data_size || to > data_size || from >= to) return NULL;
    return new Block(data + from, to - from);
}

void Block::read(void *data, std::size_t size) {
    if(size > data_size || data == NULL) return;
    std::memcpy(data, this->data, size);
    remove(0, size);
}

void Block::push_word(Word data) {
    std::size_t offset = data_size;
    /* Reserve space for one Word . . . */
    resize(data_size + AESALON_WORD_SIZE);
    
    get_data()[offset+0] = (data >> 0) & 0xff;
    get_data()[offset+1] = (data >> 8) & 0xff;
    get_data()[offset+2] = (data >> 16) & 0xff;
    get_data()[offset+3] = (data >> 24) & 0xff;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    get_data()[offset+4] = (data >> 32) & 0xff;
    get_data()[offset+5] = (data >> 40) & 0xff;
    get_data()[offset+6] = (data >> 48) & 0xff;
    get_data()[offset+7] = (data >> 56) & 0xff;
#endif
}

void Block::resize(size_t new_size) {
    if(new_size > allocated_size) {
        if(allocated_size == 0) allocated_size = 1;
        while(allocated_size < new_size) allocated_size *= 2;
        data = static_cast<Byte *>(std::realloc(data, allocated_size));
        if(data == NULL) throw Exception::OutOfMemoryException();
    }
    data_size = new_size;
}
