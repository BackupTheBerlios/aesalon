#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "Header.h"
#include "Parser.h"

namespace Aesalon {
namespace Interface {
namespace ELF {

Header::Header(int file_fd) {
    std::size_t bytes = read(file_fd, &data, sizeof(data));
    if(bytes != sizeof(data)) throw ParserException("ELF Header truncated");
    
    /* Basic validation */
    if(data.e_ident[EI_MAG0] != ELFMAG0 ||
        data.e_ident[EI_MAG1] != ELFMAG1 ||
        data.e_ident[EI_MAG2] != ELFMAG2 ||
        data.e_ident[EI_MAG3] != ELFMAG3) throw ParserException("ELF signature invalid");
    
    if(data.e_ident[EI_DATA] == ELFDATA2LSB)
        endian_mode = ENDIAN_LITTLE;
    else endian_mode = ENDIAN_BIG;
    
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    if(data.e_ident[EI_CLASS] != ELFCLASS64) throw ParserException("ELF file type is not ELFCLASS64");
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    if(data.e_ident[EI_CLASS] != ELFCLASS32) throw ParserException("ELF file type is not ELFCLASS32");
#endif
    std::cout << "ELF::Header::Header(): ELF header parsed successfully" << std::endl;
}

} // namespace ELF
} // namespace Interface
} // namespace Aesalon
