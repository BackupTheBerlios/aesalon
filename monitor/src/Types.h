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
    
    @file Types.h
    This file contains the definitions of data storage types, the sizes and
    types of which change by platform.
*/

#ifndef AESALON_MONITOR_TYPES_H
#define AESALON_MONITOR_TYPES_H

#include <sys/types.h>

#include <vector>

/** Byte typedef; simply an 8-bit integer. */
typedef u_int8_t Byte;
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
/** Unsigned word, 32-bits on 32-bit machines, 64 bits on 64-bit machines. */
typedef u_int64_t Word;
/** Signed-word, signed version of @a Word. */
typedef int64_t SWord;
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
/** Unsigned word, 32-bits on 32-bit machines, 64 bits on 64-bit machines. */
typedef u_int32_t Word;
/** Signed-word, signed version of @a Word. */
typedef int32_t SWord;
#endif

/** A block of data, presumably from an executable file. Custom version of std::vector&lt;Byte&gt;.*/
class Block {
private:
    std::vector<Byte> data;
public:
    /** Generic constructor, sets data and data_size to NULL and zero, respectively. */
    Block() : data() {}
    /** Constructor that takes a pointer and a std::size_t, for data and data_size. */
    Block(Byte *data, std::size_t data_size);
    
    /** Returns the data that this block points to, with an optional offset.
        @return The block data, @a offset bytes in.
    */
    Byte *get_data() { return &data[0]; }
    
    /** Retrieves the size of the current data.
        @return The current size of the referenced data.
    */
    std::size_t get_size() const { return data.size(); }
    /** Removes a swath of Bytes, from offset @a from to offset @a to. This invalidates all
        pointers to the current Block.
        @param from Where to begin deleting.
        @param to Where to end deleteing.
    */
    void remove(std::size_t from, std::size_t to);
    
    /** Creates a copy of a subset of the block of represented data.
        @param from Where to begin copying.
        @param to Where to end copying.
        @return The subset of the block.
    */
    Block *subset(std::size_t from, std::size_t to) const;
    
    /** Acts like the POSIX read() function -- reads a specific amount of
        data into a buffer.
        @param data The buffer to read into.
        @param size The amount of data to read.
    */
    void read(void *data, std::size_t size);
    
    void hexdump();
    
    void resize(std::size_t new_size) {
        data.resize(new_size);
    }
    
    void push_word(Word data);
};




#endif
