#include "MemoryBlockManager.h"
#include "MemoryEvent.h"

namespace Aesalon {
namespace Platform {

MemoryBlockManager::MemoryBlockManager() {

}

MemoryBlockManager::~MemoryBlockManager() {

}

void MemoryBlockManager::generate_events(Misc::EventQueue &queue) const {
    block_map_t::const_iterator i = block_map.begin();
    
    for(; i != block_map.end(); i ++) {
        queue.push_event(new MemoryBlockAllocEvent(((*i).second)->get_address(), ((*i).second)->get_size()));
    }
}

} // namespace Platform
} // namespace Aesalon
