#include "MemoryBlock.h"

namespace Aesalon {
namespace Platform {

void MemoryBlock::dump() {
    std::cout << "\tAesalon::Platform::MemoryBlock:" << std::endl;
    std::cout << "\t\tAddress: " << get_address() << std::endl;
    std::cout << "\t\tSize: " << get_size() << std::endl;
}

} // namespace Platform
} // namespace Aesalon
