#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include "Parser.h"

namespace Aesalon {
namespace Interface {
namespace ELF {

Parser::Parser(std::string filename) : filename(filename) {
    file_fd = open(filename.c_str(), O_RDONLY);
    
    header = new Header(file_fd);
    
    lseek(file_fd, header->get_section_header_offset(), SEEK_SET);
    
    std::size_t string_table_index = 0;
    for(std::size_t x = 0; x < header->get_num_sections(); x ++) {
        section_list.push_back(new Section(file_fd));
    }
    
    /* NOTE: !!! This is *EXTREMELY* unportable and probably won't work across different programs, let alone platforms . . . */
    string_table_index = header->get_num_sections() - 3;
    
    string_table = section_list[string_table_index];
    
    string_table->read_content();
    
    for(section_list_t::iterator i = section_list.begin(); i != section_list.end(); i ++) {
        char *p = string_table->get_content();
        p = p + (*i)->get_name_offset();
        (*i)->set_name(p);
        std::cout << "ELF::Parser::Parser(): Setting name of section to \"" << p << "\", offset is: " << (*i)->get_name_offset() << "\n";
        if((*i)->is_string_table()) std::cout << "\t^^ is string table" << std::endl;
    }
    
    close(file_fd);
}

} // namespace ELF
} // namespace Interface
} // namespace Aesalon
