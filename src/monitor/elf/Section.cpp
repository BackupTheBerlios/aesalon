#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "Section.h"
#include "Parser.h"
#include <iostream>

namespace Aesalon {
namespace Monitor {
namespace ELF {

Section::Section(int file_fd) : file_fd(file_fd) {
    std::size_t bytes = read(file_fd, &data, sizeof(data));
    if(bytes != sizeof(data)) throw ParserException("Section entry truncated");
}

void Section::read_content() {
    lseek(file_fd, data.sh_offset, SEEK_SET);
    Byte *byte_content = new Byte[get_content_size()];
    
    std::size_t bytes = read(file_fd, byte_content, data.sh_size);
    if(bytes != data.sh_size) throw ParserException("Section data ended unexpectedly");
    
    content.set_data(byte_content);
    content.set_size(bytes);
}

} // namespace ELF
} // namespace Monitor
} // namespace Aesalon
