#ifndef AESALON_MONITOR_TYPES_H
#define AESALON_MONITOR_TYPES_H

#include <vector>

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Monitor {

/** Byte typedef; simply an 8-bit integer. */
typedef unsigned char Byte;
/** Unsigned word, 32-bits on 32-bit machines, 64 bits on 64-bit machines. */
typedef unsigned long Word;
/** Signed-word, signed version of @a Word. */
typedef long SWord;

/** A block of data, presumably from an executable file. Custom version of std::vector&lt;Byte&gt;.*/
class Block {
    /** A pointer to the data stored in this block. */
    Byte *data;
    /** Describes how large the data that @a data points to is. */
    std::size_t data_size;
public:
    /** Generic constructor, sets data and data_size to NULL and zero, respectively. */
    Block() : data(data), data_size(0) {}
    
    /** Constructor that takes a pointer and a std::size_t, for data and data_size. */
    Block(Byte *data, std::size_t data_size) : data(data), data_size(data_size) {}
    
    /** Returns the data that this block points to, with an optional offset.
        @param offset How many bytes to ignore when determining the address.
        @return The block data, @a offset bytes in.
    */
    Byte *get_data(std::size_t offset = 0) const { return data + offset; }
    
    /** Sets the data that the block references.
        @param new_data The new data to reference.
    */
    void set_data(Byte *new_data) { data = new_data; }
    
    /** Retrieves the size of the current data.
        @return The current size of the referenced data.
    */
    std::size_t get_size() const { return data_size; }
    /** Sets the size of the referenced data.
        @param new_size The new size.
    */
    void set_size(std::size_t new_size) { data_size = new_size; }
    /** Removes a swath of Bytes, from offset @a from to offset @a to. This invalidates all
        pointers to the current Block.
        @param from Where to begin deleting.
        @param to Where to end deleteing.
    */
    void remove(std::size_t from, std::size_t to);
};

} // namespace Monitor
} // namespace Aesalon

#endif
