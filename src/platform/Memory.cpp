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

void Memory::generate_events(Misc::EventQueue &queue) const {
    block_manager->generate_events(queue);
    /*reference_manager->generate_events(queue);*/
}

} // namespace Platform
} // namespace Aesalon
