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
    
    @file asm/InstructionList.cpp
*/

#include "InstructionList.h"

namespace ASM {

std::size_t InstructionList::get_instruction_count() const {
    std::size_t count = 0;
    for(instruction_list_t::const_iterator i = instruction_list.begin(); i != instruction_list.end(); i ++) {
        if(*i) count ++;
    }
    return count;
}

StorageOffset InstructionList::get_instruction_by_index(std::size_t index) const {
    instruction_list_t::const_iterator i = instruction_list.begin();
    for(; index > 0 && i != instruction_list.end(); i ++) {
        if(*i) index --;
    }
    if(i == instruction_list.end()) return -1;
    return *i;
}

} // namespace ASM
