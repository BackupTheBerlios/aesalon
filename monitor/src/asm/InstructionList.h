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
    
    @file asm/InstructionList.h
*/

#ifndef AESALON_MONITOR_ASM_INSTRUCTION_LIST_H
#define AESALON_MONITOR_ASM_INSTRUCTION_LIST_H

#include <vector>
#include "Types.h"
#include "Instruction.h"
#include "StorageOffset.h"

namespace ASM {

class InstructionList {
protected:
    typedef std::vector<StorageOffset> instruction_list_t;
private:
    StorageOffset storage_offset;
    Word offset;
    instruction_list_t instruction_list;
public:
    InstructionList(StorageOffset storage_offset, Word offset) : storage_offset(storage_offset), offset(offset) {}
    virtual ~InstructionList() {}
    
    StorageOffset get_storage_offset() const { return storage_offset; }
    
    void add_instruction(StorageOffset new_instruction)
        { instruction_list.push_back(new_instruction); }
    StorageOffset get_instruction(Word address) const {
        if(address > offset) return -1;
        return instruction_list[address-offset];
    }
    std::size_t get_instruction_count() const;
    StorageOffset get_instruction_by_index(std::size_t index) const;
    
};

} // namespace ASM

#endif
