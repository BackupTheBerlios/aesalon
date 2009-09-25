#ifndef AESALON_INTERFACE_MEMORY_BLOCK_MANAGER_H
#define AESALON_INTERFACE_MEMORY_BLOCK_MANAGER_H

#include <vector>
#include <map>

#include "MemoryBlock.h"
#include "misc/SmartPointer.h"
#include "MemoryEvent.h"

namespace Aesalon {
namespace Interface {

class MemoryBlockManager {
public:
    typedef std::map<std::size_t, Misc::SmartPointer<MemoryBlock> > block_map_t;
private:
    block_map_t block_map;
    
    void handle_malloc_event(MallocEvent *me);
    void handle_free_event(FreeEvent *me);
public:
    MemoryBlockManager();
    virtual ~MemoryBlockManager();
    
    void handle_memory_event(MemoryEvent *me);
    
    /* NOTE: debugging purposes only. */
    void dump_memory();
};

} // namespace Interface
} // namespace Aesalon


#endif
