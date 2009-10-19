#include "MemoryBlock.h"

namespace Aesalon {
namespace Interface {

void MemoryBlock::dump() {
    std::cout << "\tAesalon::Interface::MemoryBlock:" << std::endl;
    std::cout << "\t\tAddress: " << get_address() << std::endl;
    std::cout << "\t\tSize: " << get_size() << std::endl;
}

} // namespace Interface
} // namespace Aesalon
