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
    
    @file asm/Operand.h
*/

#ifndef AESALON_MONITOR_ASM_OPERAND_H
#define AESALON_MONITOR_ASM_OPERAND_H

#include "Types.h"
#include "Register.h"
#include "StorageOffset.h"

namespace ASM {

class Operand {
public:
    enum operand_type_e {
        REGISTER,
        MEMORY_ADDRESS,
        REGISTER_OFFSET
    };
private:
    StorageOffset storage_offset;
    operand_type_e operand_type;
    Word address;
    SWord offset;
    Register reg;
    Register::data_size_e data_size;
public:
    Operand(StorageOffset storage_offset, std::string operand_string);
    Operand(StorageOffset storage_offset, operand_type_e operand_type, Word address = 0,
        Register reg = Register::INVALID, SWord offset = 0) : storage_offset(storage_offset),
        operand_type(operand_type), address(address), offset(offset), reg(reg) {}
    virtual ~Operand() {}
    
    StorageOffset get_storage_offset() const { return storage_offset; }
    
    Word get_address() const { return address; }
    void set_address(Word new_address) { address = new_address; }
    
    Register get_register() const { return reg; }
    void set_register(Register new_register) { reg = new_register; }
};

} // namespace ASM

#endif
