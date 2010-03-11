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
    
    @file ptrace/MallocObserver.cpp
*/

#include <iostream>

#include "MallocObserver.h"
#include "Initializer.h"
#include "event/BlockEvent.h"

namespace PTrace {

bool MallocObserver::handle_breakpoint(Breakpoint *breakpoint) {
    Portal *portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    
    Word size, address, ret_address;
    Word rbp = portal->get_register(ASM::Register::RBP);
    
    address = portal->read_memory(rbp-0x08);
    size = portal->read_memory(rbp-0x18);
    ret_address = portal->read_memory(rbp-0x20);
    
    /*std::cout << "Allocation detected: address is 0x" << std::hex << address << ", size is " << std::dec << size << std::endl;*/
    
    Initializer::get_instance()->get_event_queue()->push_event(new Event::BlockEvent(Event::BlockEvent::ALLOC_EVENT, address, size));
    
    return true;
}

} // namespace PTrace
