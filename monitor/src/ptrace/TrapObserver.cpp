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
    
    @file ptrace/TrapObserver.cpp
*/

#include <signal.h>
#include <iostream>

#include "TrapObserver.h"
#include "Initializer.h"

namespace PTrace {

bool TrapObserver::handle_signal(int signal, int status) {
    if(signal != SIGTRAP) return false;
    
    ProgramManager *program_manager = Initializer::get_instance()->get_program_manager();
    if(first) {
        Initializer::get_instance()->get_program_manager()->get_ptrace_portal()->get_map_parser()->parse_maps();
        Initializer::get_instance()->get_program_manager()->setup();
        first = false;
    }
    else program_manager->get_ptrace_portal()->handle_breakpoint();
    
    program_manager->get_ptrace_portal()->continue_execution();
    return true;
}

} // namespace PTrace
