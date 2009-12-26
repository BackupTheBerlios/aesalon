#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <cstring>
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
    std::cout << "Section::read_content(): file_fd is: " << file_fd << std::endl;
    lseek(file_fd, data.sh_offset, SEEK_SET);
    Byte *byte_content = new Byte[get_content_size()];
    
    std::cout << "Section::read_content(): data.sh_size is: " << data.sh_size << std::endl;
    
    ssize_t bytes = read(file_fd, byte_content, data.sh_size);
    if(bytes == -1) throw ParserException(Misc::StreamAsString() << "Error while reading section content: " << strerror(errno));
    if(bytes != data.sh_size) {
        std::cout << "Section::read_content(): only read " << bytes << " bytes . . ." << std::endl;
        throw ParserException("Section data ended unexpectedly");
    }
    
    content = new Block(byte_content, bytes);
}    

} // namespace ELF
} // namespace Monitor
} // namespace Aesalon
