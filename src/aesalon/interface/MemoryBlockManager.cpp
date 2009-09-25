#include "MemoryBlockManager.h"

namespace Aesalon {
namespace Interface {

MemoryBlockManager::MemoryBlockManager() {

}

MemoryBlockManager::~MemoryBlockManager() {

}

void MemoryBlockManager::handle_memory_event(Aesalon::Interface::MemoryEvent* me) {
    switch(me->get_memory_type()) {
        case MemoryEvent::MALLOC_EVENT:
            handle_malloc_event(dynamic_cast<MallocEvent *>(me));
            break;
        case MemoryEvent::FREE_EVENT:
            handle_free_event(dynamic_cast<FreeEvent *>(me));
        default: break;
    }
}

void MemoryBlockManager::handle_malloc_event(Aesalon::Interface::MallocEvent* me) {
    MemoryBlock *mb = new MemoryBlock(me->get_address(), me->get_size(), me->get_scope());
    block_map[me->get_address()] = mb;
}

void MemoryBlockManager::handle_free_event(Aesalon::Interface::FreeEvent* me) {
    MemoryBlock *mb = block_map[me->get_address()];
    if(mb) {
        delete mb;
        block_map[me->get_address()] = 0;
    }
}

void MemoryBlockManager::dump_memory() {
    block_map_t::iterator i = block_map.begin();
    for(; i != block_map.end() && (*i).second.is_valid(); i ++) {
        (*i).second->dump();
    }
}

} // namespace Interface
} // namespace Aesalon
