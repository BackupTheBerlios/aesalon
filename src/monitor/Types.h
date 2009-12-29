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
private:
    std::vector<Byte> data;
public:
    /** Generic constructor, sets data and data_size to NULL and zero, respectively. */
    Block() : data() {}
    /** Constructor that takes a pointer and a std::size_t, for data and data_size. */
    Block(Byte *data, std::size_t data_size);
    
    /** Returns the data that this block points to, with an optional offset.
        @param offset How many bytes to ignore when determining the address.
        @return The block data, @a offset bytes in.
    */
    Byte *get_data(std::size_t offset = 0) { return &data[offset]; }
    
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
    Misc::SmartPointer<Block> subset(std::size_t from, std::size_t to) const;
    
    /** Acts like the POSIX read() function -- reads a specific amount of
        data into a buffer.
        @param data The buffer to read into.
        @param size The amount of data to read.
    */
    void read(void *data, std::size_t size);
};

} // namespace Monitor
} // namespace Aesalon

#endif
