#ifndef AESALON_INTERFACE_MEMORY_H
#define AESALON_INTERFACE_MEMORY_H

#include <cstddef>

#include "MemoryAddress.h"
#include "MemoryBlockManager.h"
#include "misc/SmartPointer.h"

namespace Aesalon {
namespace Interface {

class Memory {
private:
    Misc::SmartPointer<MemoryBlockManager> block_manager;
public:
    Memory();
    virtual ~Memory();
    
    Misc::SmartPointer<MemoryBlockManager> get_block_manager() const { return block_manager; }
};

} // namespace Interface
} // namespace Aesalon

#endif
