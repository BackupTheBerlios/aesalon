#ifndef AESALON_PLATFORM_MEMORY_REFERENCE_MANAGER_H
#define AESALON_PLATFORM_MEMORY_REFERENCE_MANAGER_H

#include <vector>

#include "MemoryReference.h"
#include "MemoryBlock.h"
#include "misc/SmartPointer.h"
#include "misc/EventQueue.h"

namespace Aesalon {
namespace Platform {

class MemoryReferenceManager {
public:
    typedef std::vector<Misc::SmartPointer<MemoryReference> > memory_reference_vector_t;
private:
    memory_reference_vector_t memory_references;
public:
    MemoryReferenceManager();
    virtual ~MemoryReferenceManager();
    
    void add_reference(Misc::SmartPointer<MemoryReference> new_reference) {
        memory_references.push_back(new_reference);
    }
    
    std::size_t get_block_references(const Misc::SmartPointer<MemoryBlock> block) const;
    memory_reference_vector_t *get_references_by_block(const Misc::SmartPointer<MemoryBlock> block) const;
    Misc::SmartPointer<MemoryReference> get_reference_by_scope(const Misc::SmartPointer<MemoryReferenceScope> scope);
    Misc::SmartPointer<MemoryReference> get_reference_by_id(const MemoryReferenceID id) const;
    
    void generate_events(Misc::EventQueue &queue) const;
};

} // namespace Platform
} // namespace Aesalon

#endif
