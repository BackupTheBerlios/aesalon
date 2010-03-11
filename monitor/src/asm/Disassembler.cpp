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
    
    @file asm/Disassembler.cpp
*/

#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include "Disassembler.h"
#include "elf/Symbol.h"
#include "misc/ArgumentList.h"
#include "misc/String.h"
#include "misc/Message.h"
#include "Initializer.h"

namespace ASM {

Disassembler::Disassembler(ELF::Parser *elf_parser) : elf_parser(elf_parser) {
    Misc::ArgumentList al;
    al.add_argument("/usr/bin/objdump"); /* NOTE: hardcoded path . . . */
    al.add_argument("-dMintel");
    al.add_argument("--section=.text");
    al.add_argument(elf_parser->get_filename());
    
    int pipe_fds[2];
    pipe(pipe_fds);
    
    pid_t pid = fork();
    if(pid == 0) {
        close(pipe_fds[0]);
        fcntl(pipe_fds[1], F_SETFL, fcntl(pipe_fds[1], F_GETFL) & ~O_NONBLOCK);
        dup2(pipe_fds[1], STDOUT_FILENO);
        execv(al.get_argument(0).c_str(), al.get_as_argv());
        exit(1);
    }
    pipe_fd = pipe_fds[0];
    close(pipe_fds[1]);
    
    Misc::Message::Message(Misc::Message::DEBUG_MESSAGE, "Beginning disassembly of target");
    parse_objdump_output();
    Misc::Message::Message(Misc::Message::DEBUG_MESSAGE, "Disassembly of target completed");
    close(pipe_fd);
}

Disassembler::~Disassembler() {
}

void Disassembler::parse_objdump_output() {
    std::string line = "";
    ELF::Symbol *symbol = NULL;
    StorageManager *storage_manager = Initializer::get_instance()->get_storage_manager();
    
    char *buffer = new char[1024];
    /* Clear the buffer to avoid some nasty uninitialised value errors. */
    for(int x = 0; x < 1024; x ++) {
        buffer[x] = 0;
    }
    
    while(read(pipe_fd, buffer, sizeof(1024))) {
        if(!std::strlen(buffer)) continue;
        line.assign(buffer);
        if(line == "") continue;
        
        /*std::cout << "parsing objdump line \"" << line << "\"\n";*/
        
        Word address = 0;
        Misc::String::to<Word>(line, address, true);
        if(address < elf_parser->get_section(".text")->get_virtual_address()) continue;
        
        line.erase(0, line.find(" "));
        while(line[0] == ' ') line.erase(0, 1);
        
        if(line[0] == '<') {
            line.erase(0, 1);
            line.erase(line.find(">"));
            /* NOTE: ignores all symbols beginning with __ . . . */
            if(line.substr(0, 2) == "__") symbol = NULL;
            else {
                symbol = elf_parser->get_symbol(line);
                /*std::cout << "\tSymbol name is \"" << line << "\"\n";*/
            }
            continue;
        }
        /* it's an instruction . . . */
        if(!symbol) continue; /* Continue if there's no resolved symbol ATM . . . */
        if(line.find("<") != std::string::npos) line.erase(line.find("<"));
        bool finished = false;
        while(!finished) {
            Byte value = 0;
            Misc::String::to<Byte>(line.substr(0, 2), value, true);
            if(value == 0) break;
            line.erase(0, 1);
        }
        while(line.length() && line[line.length()-1] == ' ') line.erase(line.length()-1);
        while(line.length() && (line[0] == ' ' || line[0] == '\t')) {
            line.erase(0, 1);
        }
        if(line == "") continue;
        /*std::cout << "Assembly instruction is \"" << line << "\"\n";*/
        /* Now parse the instruction and push it onto the InstructionList for the symbol . . . */
        if(!symbol_to_il[symbol->get_symbol_name()])
            symbol_to_il[symbol->get_symbol_name()] = storage_manager->new_instruction_list(symbol->get_address())->get_storage_offset();
        
        storage_manager->get_instruction_list(symbol_to_il[symbol->get_symbol_name()])->add_instruction(storage_manager->new_instruction(line, address)->get_storage_offset());
        
        buffer[0] = 0;
    }
    delete[] buffer;
}

} // namespace ASM
