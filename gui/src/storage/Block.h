#ifndef AESALON_GUI_STORAGE_BLOCK_H
#define AESALON_GUI_STORAGE_BLOCK_H

#include "MemoryTypes.h"
#include "Timestamp.h"

class Block {
private:
    Timestamp allocation_time;
    Timestamp release_time;
    MemoryAddress address;
    MemorySize size;
public:
    Block(const Timestamp &allocation_time, MemoryAddress address, MemorySize size) : allocation_time(allocation_time), release_time(0), address(address), size(size) {}
    
    Block *clone();
    
    const Timestamp &get_allocation_time() const { return allocation_time; }
    const Timestamp &get_release_time() const { return release_time; }
    void set_release_time(const Timestamp &time) { release_time = time; }
    MemoryAddress get_address() const { return address; }
    MemorySize get_size() const { return size; }
};

#endif
