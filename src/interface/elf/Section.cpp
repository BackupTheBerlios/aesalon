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
}

void Section::read_content() {
    lseek(file_fd, data.sh_offset, SEEK_SET);
    content = new char[data.sh_size];
    std::size_t bytes = read(file_fd, content, data.sh_size);
    if(bytes != data.sh_size) throw ParserException("Section data ended unexpectedly");
}

} // namespace ELF
} // namespace Interface
} // namespace Aesalon
