#ifndef AESALON_INTERFACE_ELF_HEADER_H
#define AESALON_INTERFACE_ELF_HEADER_H

#include <linux/elf.h>

#include "platform/MemoryAddress.h"

namespace Aesalon {
namespace Interface {
namespace ELF {

class Header {
public:
    enum endian_mode_e {
        ENDIAN_LITTLE,
        ENDIAN_BIG
    };
private:
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    typedef Elf64_Ehdr header_t;
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    typedef Elf32_Ehdr header_t;
#endif
    header_t data;
    
    endian_mode_e endian_mode;
public:
    Header(int file_fd);
    virtual ~Header() {}
    
    Platform::MemoryAddress get_section_header_offset() const { return data.e_shoff; }
    std::size_t get_num_sections() const { return data.e_shnum; } 
    
    endian_mode_e get_endian_mode() const { return endian_mode; }
};

} // namespace ELF
} // namespace Interface
} // namespace Aesalon

#endif
