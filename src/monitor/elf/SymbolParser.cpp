#include <iostream>
#include <cstring>

#include "SymbolParser.h"
#include "Parser.h"

namespace Aesalon {
namespace Monitor {
namespace ELF {

SymbolParser::SymbolParser(Misc::SmartPointer<Parser> elf_parser) : elf_parser(elf_parser) {
    std::cout << std::hex;
    /* Start off with the static symbols . . . */
    Misc::SmartPointer<Section> symbol_table = elf_parser->get_section(".symtab");
    if(symbol_table.is_valid()) { /* Only try to parse the symbols if the .symtab section exists */
        Misc::SmartPointer<Block> symbol_block = symbol_table->get_content();
        while(symbol_block->get_size()) {
            Elf64_Sym sym;
            symbol_block->read(&sym, sizeof(sym));
            
            char *name = (char *)elf_parser->get_section(".strtab")->get_content()->get_data(sym.st_name);
            if(sym.st_value) symbol_vector.push_back(new Symbol(name, sym.st_value, sym.st_size));
        }
    }
    symbol_table = elf_parser->get_section(".rela.plt");
    if(symbol_table.is_valid()) {
        symbol_table->read_content();
        /*Misc::SmartPointer<Block> address_table = elf_parser->get_section(".rela.plt")->get_content();
        while(address_table->get_size()) {
            Elf64_Rela rela;
            address_table->read(&rela, sizeof(rela));
            Word symbol = ELF64_R_SYM(rela.r_info);
            if(symbol > dynamic_offsets.size()) dynamic_offsets.resize(symbol * 2);
            dynamic_offsets[symbol] = rela.r_offset;
            std::cout << "Dynamic offset parsed:" << std::hex << std::endl;
            std::cout << "\tsymbol: " << symbol << std::endl;
            std::cout << "\tr_offset: " << rela.r_offset << std::endl;
            std::cout << "\tr_addend: " << rela.r_addend << std::endl;
        }*/
        
        Misc::SmartPointer<Block> symbol_block;
        /* Now for the dynamic symbols. Otherwise known as the tricky ones. */
        symbol_block = elf_parser->get_section(".dynsym")->get_content();
        std::size_t index = 0;
        while(symbol_block->get_size()) {
            Elf64_Sym sym;
            symbol_block->read(&sym, sizeof(sym));
            
            char *name = (char *)elf_parser->get_section(".dynstr")->get_content()->get_data(sym.st_name);
            /*if(!std::strcmp(name, "malloc")) {
                std::cout << "malloc symbol:" << std::endl;
                std::cout << "\tdynamic_offset is: " << dynamic_offsets[index] << std::endl;
                std::cout << "\tvalue is " << sym.st_value << std::endl;
            }*/
            /*std::cout << "Dynamic symbol: name is \"" << name << "\", address is " << sym.st_value << std::endl;
            std::cout << "Dynamic offset for this symbol is: " << dynamic_offsets[index] << std::endl;*/
            if(sym.st_name) symbol_vector.push_back(new Symbol(name, sym.st_value/* + dynamic_offsets[index]*/, sym.st_size));
            index ++;
        }
    }
    std::cout << std::dec;
}

Misc::SmartPointer<Symbol> SymbolParser::get_symbol(std::string name) const {
    for(symbol_vector_t::const_iterator i = symbol_vector.begin(); i != symbol_vector.end(); i ++) {
        if((*i)->get_symbol_name() == name) return *i;
    }
    return NULL;
}

void SymbolParser::dump_symbols() const {
    std::cout << "Symbol dump: \n";
    for(symbol_vector_t::const_iterator i = symbol_vector.begin(); i != symbol_vector.end(); i ++) {
        std::cout << "\tSymbol: name is \"" << (*i)->get_symbol_name() << "\", address is 0x" << std::hex << (*i)->get_address() << std::dec << std::endl;
    }
}

} // namespace ELF
} // namespace Monitor
} // namespace Aesalon
