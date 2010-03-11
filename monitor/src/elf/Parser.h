/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file elf/Parser.h
*/

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
