#include <iostream>

#include "SymbolParser.h"
#include "Parser.h"

namespace Aesalon {
namespace Monitor {
namespace ELF {

SymbolParser::SymbolParser(Misc::SmartPointer<Parser> elf_parser) : elf_parser(elf_parser) {
    Misc::SmartPointer<Block> symbol_block = elf_parser->get_section(".symtab")->get_content();
    while(symbol_block->get_size()) {
        Elf64_Sym sym;
        symbol_block->read(&sym, sizeof(sym));
        std::string symbol_name;
        
        char *name = (char *)elf_parser->get_section(".strtab")->get_content()->get_data(sym.st_name);
        if(sym.st_value) symbol_vector.push_back(new Symbol(name, sym.st_value));
    }
}

Misc::SmartPointer<Symbol> SymbolParser::get_symbol(std::string name) const {
    for(symbol_vector_t::const_iterator i = symbol_vector.begin(); i != symbol_vector.end(); i ++) {
        if((*i)->get_symbol_name() == name) return *i;
    }
    return NULL;
}

} // namespace ELF
} // namespace Monitor
} // namespace Aesalon
