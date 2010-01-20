#include "Memory.h"

namespace Aesalon {
namespace Platform {

Misc::SmartPointer<MemoryBlock> Memory::get_block(MemoryAddress address) const {
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

} // namespace Platform
} // namespace Aesalon
