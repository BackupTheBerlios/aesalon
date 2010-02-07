#ifndef AESALON_GUI_STORAGE_BLOCK_H
#define AESALON_GUI_STORAGE_BLOCK_H

#include "MemoryTypes.h"

class Block {
private:
    MemoryAddress address;
    MemorySize size;
public:
    Block(MemoryAddress address, MemorySize size) : address(address), size(size) {}
    ~Block() {}
    
    MemoryAddress get_address() const { return address; }
    MemorySize get_size() const { return size; }
};

#endif
