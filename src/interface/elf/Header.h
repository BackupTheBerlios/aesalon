#ifndef AESALON_INTERFACE_ELF_HEADER_H
#define AESALON_INTERFACE_ELF_HEADER_H

#include <linux/elf.h>

namespace Aesalon {
namespace Interface {
namespace ELF {

class Header {
private:
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    typedef Elf64_Ehdr header_t;
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    typedef Elf32_Ehdr header_t;
#endif
public:
    
};

} // namespace ELF
} // namespace Interface
} // namespace Aesalon

#endif
