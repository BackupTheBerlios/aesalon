#include "Memory.h"

namespace Aesalon {
namespace Interface {

Memory::Memory() {
    block_manager = new MemoryBlockManager();
}

Memory::~Memory() {
    if(block_manager.is_valid()) delete block_manager;
}

} // namespace Interface
} // namespace Aesalon
