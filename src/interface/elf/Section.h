#ifndef AESALON_INTERFACE_ELF_SECTION_H
#define AESALON_INTERFACE_ELF_SECTION_H

#include <linux/elf.h>

namespace Aesalon {
namespace Interface {
namespace ELF {

class Section {
private:
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    typedef Elf64_Shdr section_t;
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    typedef Elf32_Shdr section_t;
#endif

public:
    Section() {}
    virtual ~Section() {}
    
    void parse();
};

} // namespace ELF
} // namespace Interface
} // namespace Aesalon

#endif
