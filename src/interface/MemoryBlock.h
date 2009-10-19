#ifndef AESALON_INTERFACE_MEMORY_BLOCK_H
#define AESALON_INTERFACE_MEMORY_BLOCK_H

#include <cstddef>
#include <string>
#include <iostream>

namespace Aesalon {
namespace Interface {

/** Class denoting a block of program memory. */
class MemoryBlock {
public:
    /* NOTE: this should be someplace else, it doesn't belong here . . . */
    typedef std::size_t memory_address_t;
private:
    memory_address_t address;
    std::size_t size;
public:
    MemoryBlock(std::size_t address, std::size_t size, std::string scope) :
        address(address), size(size) {
    }
    virtual ~MemoryBlock() {}
    
    void set_address(memory_address_t new_address) { address = new_address; }
    memory_address_t get_address() const { return address; }
    void set_size(std::size_t new_size) { size = new_size; }
    std::size_t get_size() const { return size; }
    
    /* NOTE: for debugging purposes . . . */
    void dump();
};

} // namespace Interface
} // namespace Aesalon

#endif
