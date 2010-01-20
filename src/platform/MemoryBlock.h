#ifndef AESALON_PLATFORM_MEMORY_BLOCK_H
#define AESALON_PLATFORM_MEMORY_BLOCK_H

#include "MemoryAddress.h"

namespace Aesalon {
namespace Platform {

class MemoryBlock {
private:
    MemoryAddress address;
    MemoryAddress size;
public:
    MemoryBlock(MemoryAddress address, MemoryAddress size) : address(address), size(size) {}
    
    MemoryAddress get_address() const { return address; }
    void set_address(MemoryAddress new_address) { address = new_address; }
    MemoryAddress get_size() const { return size; }
    void set_size(MemoryAddress new_size) { size = new_size; }
};

} // namespace Platform
} // namespace Aesalon

#endif
