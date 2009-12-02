#ifndef AESALON_PLATFORM_MEMORY_BLOCK_MANAGER_H
#define AESALON_PLATFORM_MEMORY_BLOCK_MANAGER_H

#include <vector>
#include <map>

#include "MemoryAddress.h"
#include "MemoryBlock.h"
#include "misc/SmartPointer.h"
#include "EventQueue.h"

namespace Aesalon {
namespace Platform {

class MemoryBlockManager {
public:
    typedef std::map<std::size_t, Misc::SmartPointer<MemoryBlock> > block_map_t;
private:
    block_map_t block_map;
public:
    MemoryBlockManager();
    virtual ~MemoryBlockManager();
    
    void add_block(Misc::SmartPointer<MemoryBlock> block) { block_map[block->get_address()] = block; }
    Misc::SmartPointer<MemoryBlock> get_block(MemoryAddress address) { return block_map[address]; }
    
    /* NOTE: debugging purposes only. */
    void dump_memory();
    
    void generate_events(Misc::SmartPointer<EventQueue> queue) const;
};

} // namespace Platform
} // namespace Aesalon


#endif
