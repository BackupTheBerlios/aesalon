#include <iostream>
#include <cstring>

#include "SymbolParser.h"
#include "Parser.h"
#include "Initializer.h"

namespace ELF {

SymbolParser::SymbolParser(Parser *elf_parser) : elf_parser(elf_parser) {
    StorageManager *storage_manager = Initializer::get_instance()->get_storage_manager();
    std::cout << std::hex;
    /* Start off with the static symbols . . . */
    Section *symbol_table = elf_parser->get_section(".symtab");
    if(symbol_table) { /* Only try to parse the symbols if the .symtab section exists */
        Block *symbol_block = symbol_table->get_content();
        while(symbol_block->get_size()) {
            Elf64_Sym sym;
            symbol_block->read(&sym, sizeof(sym));
            
            char *name = (char *)(elf_parser->get_section(".strtab")->get_content()->get_data() + sym.st_name);
            if(sym.st_value) symbol_vector.push_back(storage_manager->new_symbol(name, sym.st_value, sym.st_size)->get_storage_offset());
        }
    }
    symbol_table = elf_parser->get_section(".rela.plt");
    if(symbol_table) {
        symbol_table->read_content();
        /*Block *address_table = elf_parser->get_section(".rela.plt")->get_content();
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
        
        Block *symbol_block;
        /* Now for the dynamic symbols. Otherwise known as the tricky ones. */
        symbol_block = elf_parser->get_section(".dynsym")->get_content();
        std::size_t index = 0;
        while(symbol_block->get_size()) {
            Elf64_Sym sym;
            symbol_block->read(&sym, sizeof(sym));
            
            char *name = (char *)&elf_parser->get_section(".dynstr")->get_content()->get_data()[sym.st_name];
            /*if(!std::strcmp(name, "malloc")) {
                std::cout << "malloc symbol:" << std::endl;
                std::cout << "\tdynamic_offset is: " << dynamic_offsets[index] << std::endl;
                std::cout << "\tvalue is " << sym.st_value << std::endl;
            }*/
            /*std::cout << "Dynamic symbol: name is \"" << name << "\", address is " << sym.st_value << std::endl;
            std::cout << "Dynamic offset for this symbol is: " << dynamic_offsets[index] << std::endl;*/
            if(sym.st_name) symbol_vector.push_back(storage_manager->new_symbol(name, sym.st_value/* + dynamic_offsets[index]*/, sym.st_size)->get_storage_offset());
            index ++;
        }
    }
    std::cout << std::dec;
}

SymbolParser::~SymbolParser() {
    for(symbol_vector_t::iterator i = symbol_vector.begin(); i != symbol_vector.end(); i ++) {
        Initializer::get_instance()->get_storage_manager()->get_symbol(*i)->~Symbol();
    }
}

Symbol *SymbolParser::get_symbol(std::string name) const {
    StorageManager *storage_manager = Initializer::get_instance()->get_storage_manager();
    for(symbol_vector_t::const_iterator i = symbol_vector.begin(); i != symbol_vector.end(); i ++) {
        if(storage_manager->get_symbol(*i)->get_symbol_name() == name) return storage_manager->get_symbol(*i);
    }
    return NULL;
}

Symbol *SymbolParser::get_symbol(Word address) const {
    StorageManager *storage_manager = Initializer::get_instance()->get_storage_manager();
    for(symbol_vector_t::const_iterator i = symbol_vector.begin(); i != symbol_vector.end(); i ++) {
        Symbol *symbol = storage_manager->get_symbol(*i);
        if(symbol->get_address() <= address && address < symbol->get_address() + symbol->get_size()) {
            return symbol;
        }
    }
    return NULL;
}


void SymbolParser::dump_symbols() const {
    std::cout << "Symbol dump: \n";
    for(symbol_vector_t::const_iterator i = symbol_vector.begin(); i != symbol_vector.end(); i ++) {
        /*std::cout << "\tSymbol: name is \"" << (*i)->get_symbol_name() << "\", address is 0x" << std::hex << (*i)->get_address() << std::dec << std::endl;*/
    }
}

} // namespace ELF
