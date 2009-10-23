#include "Memory.h"

namespace Aesalon {
namespace Platform {

Memory::Memory() {
    block_manager = new MemoryBlockManager();
}

Memory::~Memory() {
    if(block_manager.is_valid()) delete block_manager;
}

} // namespace Platform
} // namespace Aesalon
