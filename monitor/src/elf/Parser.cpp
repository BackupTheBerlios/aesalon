#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include "Parser.h"

namespace ELF {

Parser::Parser(std::string filename) : filename(filename) {
    std::cout << "ELF::Parser::Parser(): filename is \"" << filename << "\"\n";
    file_fd = open(filename.c_str(), O_RDONLY);
    
    header = new Header(file_fd);
    
    lseek(file_fd, header->get_section_header_offset(), SEEK_SET);
    
    for(std::size_t x = 0; x < header->get_num_sections(); x ++) {
        section_list.push_back(new Section(file_fd));
    }
    
    /* Remove the first section, since, according to the standard, it is "reserved and should be all zeros" */
    /*section_list.erase(section_list.begin());*/
    
    string_table = section_list[header->get_string_table_index()];
    
    string_table->read_content();
    
    for(section_list_t::iterator i = section_list.begin(); i != section_list.end(); i ++) {
        Byte *p = string_table->get_content()->get_data();
        p += (*i)->get_name_offset();
        
        /*std::cout << "Section, name is \"" << (char *)p << "\'\n";*/
        
        (*i)->set_name((char *)p);
    }
    
    symbol_parser = new SymbolParser(this);
}

Parser::~Parser() {
    delete header;
    delete symbol_parser;
    for(section_list_t::iterator i = section_list.begin(); i != section_list.end(); i ++) {
        delete *i;
    }
    close(file_fd);
}

Section *Parser::get_section(std::string name) const {
    for(section_list_t::const_iterator i = section_list.begin(); i != section_list.end(); i ++) {
        if((*i)->get_name() == name) return *i;
    }
    return NULL;
}

Symbol *Parser::get_symbol(std::string name) const {
    return symbol_parser->get_symbol(name);
}

} // namespace ELF