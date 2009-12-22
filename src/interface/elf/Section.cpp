#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "Section.h"
#include "Parser.h"
#include <iostream>

namespace Aesalon {
namespace Interface {
namespace ELF {

Section::Section(int file_fd) : file_fd(file_fd) {
    std::size_t bytes = read(file_fd, &data, sizeof(data));
    if(bytes != sizeof(data)) throw ParserException("Section entry truncated");
    std::cout << "Section::Section(): data.sh_offset: " << data.sh_offset << std::endl;
}

void Section::read_content() {
    std::cout << "Section::read_content(): data.sh_offset:" << data.sh_offset << std::endl;
    lseek(file_fd, data.sh_offset, SEEK_SET);
    byte_content = new Byte[get_content_size()];
    
    std::size_t bytes = read(file_fd, byte_content, data.sh_size);
    if(bytes != data.sh_size) throw ParserException("Section data ended unexpectedly");
    
    for(char *p = (char *)byte_content.operator Byte*(); (p-(char *)byte_content.operator Byte*()) < data.sh_size; p ++) {
        content.push_back(*p);
    }
}

} // namespace ELF
} // namespace Interface
} // namespace Aesalon
