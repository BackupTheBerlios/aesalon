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
    
    @file elf/Header.h
*/

#ifndef AESALON_MONITOR_ELF_HEADER_H
#define AESALON_MONITOR_ELF_HEADER_H

/* Some variants of linux/elf.h require linux/types.h . . . */
#include <sys/types.h>
#include <linux/elf.h>

#include "Types.h"

namespace ELF {

class Header {
public:
    enum endian_mode_e {
        ENDIAN_LITTLE,
        ENDIAN_BIG
    };
    
    enum elf_type_e {
        ELF32,
        ELF64
    };
private:
    endian_mode_e endian_mode;
    elf_type_e elf_type;
    
    Word section_header_offset;
    Word section_count;
    Word string_table_index;
public:
    Header(int file_fd);
    virtual ~Header() {}
    
    Word get_section_header_offset() const { return section_header_offset; }
    Word get_section_count() const { return section_count; } 
    
    Word get_string_table_index() const { return string_table_index; }
    
    endian_mode_e get_endian_mode() const { return endian_mode; }
    elf_type_e get_elf_type() const { return elf_type; }
};

} // namespace ELF

#endif
