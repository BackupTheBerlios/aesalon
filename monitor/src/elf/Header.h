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

#include <linux/elf.h>

#include "Types.h"

namespace ELF {

class Header {
public:
    enum endian_mode_e {
        ENDIAN_LITTLE,
        ENDIAN_BIG
    };
private:
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    typedef Elf64_Ehdr header_t;
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    typedef Elf32_Ehdr header_t;
#endif
    header_t data;
    
    endian_mode_e endian_mode;
public:
    Header(int file_fd);
    virtual ~Header() {}
    
    Word get_section_header_offset() const { return data.e_shoff; }
    std::size_t get_num_sections() const { return data.e_shnum; } 
    
    std::size_t get_string_table_index() const { return data.e_shstrndx; }
    
    endian_mode_e get_endian_mode() const { return endian_mode; }
};

} // namespace ELF

#endif
