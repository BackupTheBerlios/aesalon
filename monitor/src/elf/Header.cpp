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
    
    @file elf/Header.cpp
*/

#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "Header.h"
#include "Parser.h"

namespace ELF {

Header::Header(int file_fd) {
    std::size_t bytes = read(file_fd, &data, sizeof(data));
    if(bytes != sizeof(data)) throw ParserException("ELF Header truncated");
    
    /* Basic validation */
    if(data.e_ident[EI_MAG0] != ELFMAG0 ||
        data.e_ident[EI_MAG1] != ELFMAG1 ||
        data.e_ident[EI_MAG2] != ELFMAG2 ||
        data.e_ident[EI_MAG3] != ELFMAG3) throw ParserException("ELF signature invalid");
    
    /* NOTE: for now, trying to parse libc, so don't throw exception . . . */
    /*if(data.e_type != ET_EXEC) throw ParserException("Non-executable ELF file given");*/
    
    if(data.e_ident[EI_DATA] == ELFDATA2LSB)
        endian_mode = ENDIAN_LITTLE;
    else endian_mode = ENDIAN_BIG;
    
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    if(data.e_ident[EI_CLASS] != ELFCLASS64) throw ParserException("ELF file type is not ELFCLASS64");
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    if(data.e_ident[EI_CLASS] != ELFCLASS32) throw ParserException("ELF file type is not ELFCLASS32");
#endif
}

} // namespace ELF
