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
    
    @file elf/Symbol.h
*/

#ifndef AESALON_MONITOR_ELF_SYMBOL_H
#define AESALON_MONITOR_ELF_SYMBOL_H

#include <string>

#include "Types.h"
#include "StorageOffset.h"

namespace ELF {

class Symbol {
private:
    StorageOffset storage_offset;
    std::string symbol_name;
    Word address;
    Word size;
public:
    Symbol(StorageOffset storage_offset, std::string symbol_name, Word address, Word size) : storage_offset(storage_offset), symbol_name(symbol_name), address(address), size(size) {}
    virtual ~Symbol() {}
    
    StorageOffset get_storage_offset() const { return storage_offset; }
    
    Word get_address() const { return address; }
    std::string get_symbol_name() const { return symbol_name; }
    Word get_size() const { return size; }
};

} // namespace ELF

#endif
