#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "Parser.h"

namespace Aesalon {
namespace Interface {
namespace ELF {

Parser::Parser(std::string filename) : filename(filename) {
    file_fd = open(filename.c_str(), O_RDONLY);
    
    /*read(file_fd, &header, sizeof(header));
    
    if(header.e_shentsize != sizeof(section_t)) throw ParserException("Invalid ELF file");
    
    lseek(file_fd, header.e_shoff, SEEK_SET);
    int x = 0;
    for(x = 0; x < header.e_shnum; x ++) {
        Misc::SmartPointer<Section> s = new Section(file_fd);
        section_list.push_back(s);
    }
    
    section_list_t::size_type string_table_index = 0;
    
    for(; string_table_index < section_list.size(); string_table_index ++) {
        if(section_list[string_table_index].sh_type == SHT_STRTAB) break;
    }
    if(string_table_index == section_list.size()) throw ParserException("Couldn't find string table");
    
    section_t *string_table = &section_list[string_table_index];
    
    */
}

} // namespace ELF
} // namespace Interface
} // namespace Aesalon
