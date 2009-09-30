#include <iostream>
#include "ReferenceCounter.h"

namespace Aesalon {
namespace Misc {

template<> ReferenceCounter *Singleton<ReferenceCounter>::instance = 0;

template<typename Type>
void ReferenceCounter::increment_block(const Type *data) {
    if(data == NULL) return;
    
    block_map_t::iterator i = block_map.find(data);
    if(i == block_map.end()) {
        MemoryBlock new_block;
        new_block.set_refcount(1);
        block_map[data] = new_block;
    }
    else {
        (*i).second.set_refcount((*i).second.get_refcount()+1);
    }
}

template<typename Type>
void ReferenceCounter::decrement_block(const Type *data) {
    if(data == NULL) return;
    
    block_map_t::iterator i = block_map.find(data);
    if(i == block_map.end()) {
        /* This memory isn't currently in the counter . . . */
        return;
    }
    else {
        (*i).second.set_refcount((*i).second.get_refcount()-1);
    }
    if((*i).second.get_refcount() == 0) {
        std::cout << "Freeing memory due to refcount 0." << std::endl;
        delete *data;
    }
}

template<typename Type>
ReferenceCounter::refcount_t ReferenceCounter::get_block_references(const Type* data) {
    block_map_t::iterator i = block_map.find(data);
    if(i == block_map.end()) return 0;
    else return (*i).second.get_refcount();
}

} // namespace Misc
} // namespace Aesalon
