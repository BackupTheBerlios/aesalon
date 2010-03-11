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
    
    @file elf/SymbolParser.h
*/

#ifndef AESALON_MONITOR_ELF_SYMBOL_PARSER_H
#define AESALON_MONITOR_ELF_SYMBOL_PARSER_H

#include <vector>

#include "Symbol.h"
#include "StorageManager.h"

namespace ELF {

class Parser;

class SymbolParser {
protected:
    typedef std::vector<StorageOffset> symbol_vector_t;
    typedef std::vector<Word> dynamic_offsets_t;
private:
    Parser *elf_parser;
    symbol_vector_t symbol_vector;
    dynamic_offsets_t dynamic_offsets;
public:
    SymbolParser(Parser *elf_parser);
    virtual ~SymbolParser();
    
    Symbol *get_symbol(std::string name) const;
    
    void dump_symbols() const;
};

} // namespace ELF

#endif
