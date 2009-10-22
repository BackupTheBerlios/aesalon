#ifndef AESALON_INTERFACE_MEMORY_REFERENCE_H
#define AESALON_INTERFACE_MEMORY_REFERENCE_H

#include "MemoryBlock.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {

class MemoryReference {
private:
    Misc::SmartPointer<MemoryBlock> block;
public:
    MemoryReference();
    virtual ~MemoryReference();
    
    Misc::SmartPointer<MemoryBlock> get_block() const { return block; }
    void set_block(Misc::SmartPointer<MemoryBlock> new_block) { block = new_block; }
};

} // namespace Interface
} // namespace Aesalon

#endif
