#ifndef AESALON_MONITOR_ELF_PARSER_H
#define AESALON_MONITOR_ELF_PARSER_H

#include <string>
#include <vector>
#include <linux/elf.h>

#include "Section.h"
#include "Header.h"
#include "Symbol.h"
#include "SymbolParser.h"

#include "exception/BasicException.h"

namespace ELF {

class ParserException : public Exception::BasicException {
public:
    ParserException(std::string message) : Exception::BasicException(message) {}
    virtual ~ParserException() {}
};

class Parser {
public:
    typedef std::vector<Section *> section_list_t;
private:
    std::string filename;
    
    Header *header;
    
    section_list_t section_list;
    Section *string_table;
    
    SymbolParser *symbol_parser;
    
    int file_fd;
public:
    Parser(std::string filename);
    virtual ~Parser();
    
    Section *get_section(std::string name) const;
    
    Symbol *get_symbol(std::string name) const;
    void dump_symbols() const { symbol_parser->dump_symbols(); }
    
    std::string get_filename() const { return filename; }
};

} // namespace ELF

#endif
