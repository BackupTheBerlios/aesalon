#ifndef AESALON_INTERFACE_MEMORY_REFERENCE_MANAGER_H
#define AESALON_INTERFACE_MEMORY_REFERENCE_MANAGER_H

#include <vector>

#include "MemoryReference.h"
#include "MemoryBlock.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {

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
    memory_reference_vector_t *get_references_by_block(const Misc::SmartPointer<MemoryBlock> block) const;
    std::size_t get_block_references(const Misc::SmartPointer<MemoryBlock> block) const;
};

} // namespace Interface
} // namespace Aesalon


#endif
