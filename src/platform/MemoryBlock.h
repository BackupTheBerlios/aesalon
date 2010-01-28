#ifndef AESALON_PLATFORM_MEMORY_BLOCK_H
#define AESALON_PLATFORM_MEMORY_BLOCK_H

#include "MemoryAddress.h"

namespace Aesalon {
namespace Platform {

/** A representation of a block of allocated memory. */
class MemoryBlock {
private:
    /** The address of the block. */
    MemoryAddress address;
    /** The size of the block. */
    MemoryAddress size;
public:
    /** Generic constructor.
        @param address The address of the block.
        @param size The size of the block.
    */
    MemoryBlock(MemoryAddress address, MemoryAddress size) : address(address), size(size) {}
    
    /** Gets the address of the current block.
        @return The address of the block.
    */
    MemoryAddress get_address() const { return address; }
    /** Changes the address of the current block.
        @param new_addres The new address.
    */
    void set_address(MemoryAddress new_address) { address = new_address; }
    /** Returns the current size of the current block.
        @return The current size.
    */
    MemoryAddress get_size() const { return size; }
    /** Changes the size of the current block.
        @param new_size The new size.
    */
    void set_size(MemoryAddress new_size) { size = new_size; }
};

} // namespace Platform
} // namespace Aesalon

#endif
