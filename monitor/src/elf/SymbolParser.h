#ifndef AESALON_MONITOR_ELF_SYMBOL_PARSER_H
#define AESALON_MONITOR_ELF_SYMBOL_PARSER_H

#include <vector>

#include "Symbol.h"





class Parser;

class SymbolParser {
protected:
    typedef std::vector<Symbol *> symbol_vector_t;
    typedef std::vector<Word> dynamic_offsets_t;
private:
    Parser *elf_parser;
    symbol_vector_t symbol_vector;
    dynamic_offsets_t dynamic_offsets;
public:
    SymbolParser(Parser *elf_parser);
    virtual ~SymbolParser() {}
    
    Symbol *get_symbol(std::string name) const;
    
    void dump_symbols() const;
};





#endif
