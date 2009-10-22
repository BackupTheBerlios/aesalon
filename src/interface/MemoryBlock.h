#ifndef AESALON_INTERFACE_MEMORY_BLOCK_H
#define AESALON_INTERFACE_MEMORY_BLOCK_H

#include <cstddef>
#include <string>
#include <iostream>

#include "MemoryAddress.h"

namespace Aesalon {
namespace Interface {

/** Class denoting a block of program memory. */
class MemoryBlock {
private:
    MemoryAddress address;
    std::size_t size;
public:
    MemoryBlock(std::size_t address, std::size_t size, std::string scope) :
        address(address), size(size) {
    }
    virtual ~MemoryBlock() {}
    
    void set_address(MemoryAddress new_address) { address = new_address; }
    MemoryAddress get_address() const { return address; }
    void set_size(std::size_t new_size) { size = new_size; }
    std::size_t get_size() const { return size; }
    
    bool operator==(const MemoryBlock &other) const {
        if(other.address == address && other.size == size) return true;
        return false;
    }
    
    /* NOTE: for debugging purposes . . . */
    void dump();
};

} // namespace Interface
} // namespace Aesalon

#endif
