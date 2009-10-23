#include "Memory.h"

namespace Aesalon {
namespace Platform {

Memory::Memory() {
    block_manager = new MemoryBlockManager();
    reference_manager = new MemoryReferenceManager();
}

Memory::~Memory() {
    if(block_manager.is_valid()) delete block_manager;
    if(reference_manager.is_valid()) delete reference_manager;
}

} // namespace Platform
} // namespace Aesalon
