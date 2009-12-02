#ifndef AESALON_PLATFORM_MEMORY_H
#define AESALON_PLATFORM_MEMORY_H

#include <cstddef>

#include "MemoryAddress.h"
#include "MemoryBlockManager.h"
#include "MemoryReferenceManager.h"
#include "misc/SmartPointer.h"
#include "EventQueue.h"

namespace Aesalon {
namespace Platform {

class Memory {
private:
    Misc::SmartPointer<MemoryBlockManager> block_manager;
    Misc::SmartPointer<MemoryReferenceManager> reference_manager;
public:
    Memory();
    virtual ~Memory();
    
    Misc::SmartPointer<MemoryBlockManager> get_block_manager() const { return block_manager; }
    Misc::SmartPointer<MemoryReferenceManager> get_reference_manager() const { return reference_manager; }
    
    void generate_events(Misc::SmartPointer<EventQueue> queue) const;
};

} // namespace Platform
} // namespace Aesalon

#endif
