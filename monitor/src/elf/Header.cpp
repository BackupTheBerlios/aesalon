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
#include <cstring>

#include "Header.h"
#include "Parser.h"

#include "misc/Message.h"

namespace ELF {

Header::Header(int file_fd) {
    /*std::size_t bytes = read(file_fd, &data, sizeof(data));
    if(bytes != sizeof(data)) throw ParserException("ELF Header truncated");*/
    
    char ident[EI_NIDENT];
    int bytes = read(file_fd, ident, sizeof(ident));
    if(bytes != sizeof(ident)) throw ParserException("ELF Header truncated");
    
    /* Basic validation */
    if(ident[EI_MAG0] != ELFMAG0 ||
        ident[EI_MAG1] != ELFMAG1 ||
        ident[EI_MAG2] != ELFMAG2 ||
        ident[EI_MAG3] != ELFMAG3) throw ParserException("ELF signature invalid");
    

    if(ident[EI_CLASS] == ELFCLASS32) elf_type = ELF32;
    else if(ident[EI_CLASS] == ELFCLASS64) elf_type = ELF64;
    else throw ParserException("Unknown ELF class detected, not ELF32 or ELF64");
    
    if(ident[EI_DATA] == ELFDATA2LSB) endian_mode = ENDIAN_LITTLE;
    else endian_mode = ENDIAN_BIG;
    
    if(elf_type == ELF32) {
        Elf32_Ehdr header;
        lseek(file_fd, 0, SEEK_SET);
        bytes = read(file_fd, &header, sizeof(header));
        
    }
    else if(elf_type == ELF64) {
        Elf64_Ehdr header;
        lseek(file_fd, 0, SEEK_SET);
        bytes = read(file_fd, &header, sizeof(header));
    }
}

} // namespace ELF
