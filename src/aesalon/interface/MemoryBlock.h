#ifndef AESALON_INTERFACE_MEMORY_BLOCK_H
#define AESALON_INTERFACE_MEMORY_BLOCK_H

#include <cstddef>
#include <string>
#include <iostream>

namespace Aesalon {
namespace Interface {

class MemoryBlock {
private:
    std::size_t address;
    std::size_t size;
    
    std::string alloc_scope;
public:
    MemoryBlock(std::size_t address, std::size_t size, std::string scope) :
        address(address), size(size), alloc_scope(scope) {
    }
    virtual ~MemoryBlock() {}
    
    void set_scope(std::string new_scope) { alloc_scope = new_scope; }
    std::string get_scope() const { return alloc_scope; }    
    void set_address(std::size_t new_address) { address = new_address; }
    std::size_t get_address() const { return address; }
    void set_size(std::size_t new_size) { size = new_size; }
    std::size_t get_size() const { return size; }
    
    /* NOTE: for debugging purposes . . . */
    void dump();
};

} // namespace Interface
} // namespace Aesalon

#endif
