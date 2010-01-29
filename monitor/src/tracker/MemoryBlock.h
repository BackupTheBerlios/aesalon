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
        @param new_addres The new address.
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
