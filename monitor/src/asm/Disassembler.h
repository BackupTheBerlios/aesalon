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
    
    @file asm/Disassembler.h
*/

#ifndef AESALON_MONITOR_ASM_DISASSEMBLER_H
#define AESALON_MONITOR_ASM_DISASSEMBLER_H

#include <map>

#include "elf/Parser.h"
#include "elf/Symbol.h"
#include "InstructionList.h"
#include "StorageOffset.h"

namespace ASM {

class Disassembler {
public:
    typedef std::map<std::string, StorageOffset> symbol_to_il_t;
private:
    ELF::Parser *elf_parser;
    
    symbol_to_il_t symbol_to_il;
    
    int pipe_fd;
    
    void parse_objdump_output();
public:
    Disassembler(ELF::Parser *elf_parser);
    virtual ~Disassembler();
    
    StorageOffset get_symbol_il(std::string symbol_name) const { return symbol_to_il.at(symbol_name); }
};

} // namespace ASM

#endif
