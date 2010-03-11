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
    
    @file asm/Instruction.h
*/

#ifndef AESALON_MONITOR_ASM_INSTRUCTION_H
#define AESALON_MONITOR_ASM_INSTRUCTION_H

#include "Types.h"
#include "Operand.h"
#include "StorageManager.h"

namespace ASM {

class Instruction {
private:
    StorageOffset storage_offset;
    bool memory_change;
    StorageOffset source;
    StorageOffset destination;
    
    Word address;
    
    void handle_opcode(std::string opcode, std::string operands);
public:
    Instruction(StorageOffset storage_offset, std::string instruction, Word address);
    virtual ~Instruction();
    
    StorageOffset get_storage_offset() const { return storage_offset; }
    
    bool changes_memory() const { return memory_change; }
    
    StorageOffset get_source() const { return source; }
    StorageOffset get_destination() const { return destination; }
    
    Word get_address() const { return address; }
};

} // namespace ASM

#endif
