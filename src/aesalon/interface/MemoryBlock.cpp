#include "MemoryBlock.h"

namespace Aesalon {
namespace Interface {

void MemoryBlock::dump() {
    std::cout << "\tMemoryBlock:" << std::endl;
    std::cout << "\t\tAddress: " << get_address() << std::endl;
    std::cout << "\t\tSize: " << get_size() << std::endl;
    std::cout << "\t\tAllocation scope: " << get_scope() << std::endl;
}

} // namespace Interface
} // namespace Aesalon
