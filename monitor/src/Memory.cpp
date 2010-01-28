#include "Memory.h"
#include "misc/BlockEvent.h"

namespace Aesalon {
namespace Monitor {

Misc::SmartPointer<MemoryBlock> Memory::get_block(Word address) const {
    for(block_set_t::const_iterator i = block_set.begin(); i != block_set.end(); i ++) {
        if((*i)->get_address() == address) return *i;
    }
    return NULL;
}

void Memory::remove_block(Misc::SmartPointer<MemoryBlock> block) {
    for(block_set_t::iterator i = block_set.begin(); i != block_set.end(); i ++) {
        if((*i) != block) continue;
        block_set.erase(i);
        break;
    }
}

void Memory::handle_event(Misc::SmartPointer<Misc::Event> event) {
    if(event->get_type() == Misc::Event::BLOCK_EVENT) {
        Misc::SmartPointer<Misc::BlockEvent> be = event.to<Misc::BlockEvent>();
        switch(be->get_block_type()) {
            case Misc::BlockEvent::ALLOC_EVENT:
                add_block(new MemoryBlock(be->get_address(), be->get_size()));
                break;
            case Misc::BlockEvent::FREE_EVENT:
                remove_block(get_block(be->get_address()));
                break;
            case Misc::BlockEvent::REALLOC_EVENT:
                Misc::SmartPointer<MemoryBlock> block = get_block(be->get_address());
                block->set_address(be->get_new_address());
                block->set_size(be->get_size());
                break;
        }
    }
}

} // namespace Platform
} // namespace Aesalon
