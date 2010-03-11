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
    
    @file elf/Section.h
*/

#ifndef AESALON_MONITOR_ELF_SECTION_H
#define AESALON_MONITOR_ELF_SECTION_H

#include <linux/elf.h>

#include <string>

#include "Types.h"

namespace ELF {

class Section {
private:
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    typedef Elf64_Shdr section_t;
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    typedef Elf32_Shdr section_t;
#endif
    
    int file_fd;

    section_t data;
    
    Block *content;
    
    std::string name;
    
    std::size_t get_content_size() const { return data.sh_size; }
public:
    Section(int file_fd);
    virtual ~Section();
    
    void read_content();
    Block *get_content() {
        if(content == NULL) read_content();
        return content;
    }
    
    std::string get_name() const { return name; }
    void set_name(std::string new_name) { name = new_name; }

    Word get_name_offset() const { return data.sh_name; }
    Word get_data_offset() const { return data.sh_offset; }
    
    Word get_virtual_address() const { return data.sh_addr; }
    
    bool is_string_table() const { return data.sh_type == SHT_STRTAB; }
};

} // namespace ELF

#endif
