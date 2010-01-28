#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <cstring>
#include "Section.h"
#include "Parser.h"
#include <iostream>





Section::Section(int file_fd) : file_fd(file_fd) {
    std::size_t bytes = read(file_fd, &data, sizeof(data));
    if(bytes != sizeof(data)) throw ParserException("Section entry truncated");
}

void Section::read_content() {
    lseek(file_fd, data.sh_offset, SEEK_SET);
    Byte *byte_content = new Byte[get_content_size()];
    
    ssize_t bytes = read(file_fd, byte_content, data.sh_size);
    if(bytes == -1) throw ParserException(Misc::StreamAsString() << "Error while reading section content: " << strerror(errno));
    if(bytes != ssize_t(data.sh_size)) {
        throw ParserException("Section data ended unexpectedly");
    }
    
    content = new Block(byte_content, bytes);
}    




