#include "MemoryReferenceManager.h"
#include "MemoryEvent.h"

namespace Aesalon {
namespace Platform {

MemoryReferenceManager::MemoryReferenceManager() {

}

MemoryReferenceManager::~MemoryReferenceManager() {

}

std::size_t MemoryReferenceManager::get_block_references(const Misc::SmartPointer<MemoryBlock> block) const {
    std::size_t references = 0;
    memory_reference_vector_t::const_iterator mfi = memory_references.begin();
    for(; mfi != memory_references.end(); mfi ++) {
        if((*mfi)->get_block() == block) references ++;
    }
    return references;
}

MemoryReferenceManager::memory_reference_vector_t
    *MemoryReferenceManager::get_references_by_block(
    const Misc::SmartPointer<MemoryBlock> block) const {
    
    memory_reference_vector_t::const_iterator mfi = memory_references.begin();
    memory_reference_vector_t *references = new memory_reference_vector_t;
    
    for(; mfi != memory_references.end(); mfi ++) {
        if((*mfi)->get_block() == block) references->push_back(*mfi);
    }
    
    if(!references->size()) {
        delete references;
        references = NULL;
    }
    return references;
}

Misc::SmartPointer<MemoryReference> MemoryReferenceManager::get_reference_by_id(const MemoryReferenceID id) const {
    memory_reference_vector_t::const_iterator mfi = memory_references.begin();
    for(; mfi != memory_references.end(); mfi ++) {
        if((*mfi)->get_id() == id) return *mfi;
    }
    return NULL;
}

Misc::SmartPointer<MemoryReference> MemoryReferenceManager::get_reference_by_scope(
    const Misc::SmartPointer<MemoryReferenceScope> scope) {
    
    memory_reference_vector_t::const_iterator mfi = memory_references.begin();
    for(; mfi != memory_references.end(); mfi ++) {
        if((*mfi)->compare_scope(scope)) return *mfi;
    }
    return NULL;
}

void MemoryReferenceManager::generate_events(Misc::SmartPointer<EventQueue> queue) const {
    memory_reference_vector_t::const_iterator mfi = memory_references.begin();
    
    for(; mfi != memory_references.end(); mfi ++) {
        /* NOTE: put this back in later . . . */
        /*queue->push_event(new MemoryReferenceNewEvent((*mfi)->get_scope(), (*mfi)->get_block()));*/
    }
}

} // namespace Platform
} // namespace Aesalon
