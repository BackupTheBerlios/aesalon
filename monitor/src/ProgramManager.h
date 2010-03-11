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
    
    @file Initializer.h
    This file contains the definition of the ProgramManager class.
*/
#ifndef AESALON_MONITOR_PROGRAM_MANAGER_H
#define AESALON_MONITOR_PROGRAM_MANAGER_H

#include "misc/ArgumentList.h"
#include "ptrace/Portal.h"
#include "elf/Parser.h"
#include "asm/Disassembler.h"

class ProgramManager {
private:
    Misc::ArgumentList *argument_list;
    
    bool running;
    
    PTrace::Portal *ptrace_portal;
    
    ELF::Parser *elf_parser, *libc_parser, *overload_parser;
    ASM::Disassembler *disassembler;
    
    std::size_t malloc_breakpoint_id;
    std::size_t free_breakpoint_id;
    std::size_t realloc_breakpoint_id;
public:
    ProgramManager(Misc::ArgumentList *argument_list);
    virtual ~ProgramManager();
    
    void execute();
    void wait();
    
    /* called from a PTrace signal observer when the first SIGTRAP is caught, e.g. the child is loaded into memory */
    void place_initial_breakpoints();
    
    PTrace::Portal *get_ptrace_portal() const { return ptrace_portal; }
    ELF::Parser *get_elf_parser() const { return elf_parser; }
    ELF::Parser *get_libc_parser() const { return libc_parser; }
    
    Misc::ArgumentList *get_argument_list() const { return argument_list; }
    
    void set_running(bool new_running) { running = new_running; }
    bool is_running() const { return running; }
    
    std::size_t get_malloc_breakpoint_id() const { return malloc_breakpoint_id; }
    std::size_t get_free_breakpoint_id() const { return free_breakpoint_id; }
    std::size_t get_realloc_breakpoint_id() const { return realloc_breakpoint_id; }    
};




#endif
