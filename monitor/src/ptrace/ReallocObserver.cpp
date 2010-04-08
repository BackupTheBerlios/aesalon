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
    
    @file ptrace/ReallocObserver.cpp
*/

#include <iostream>
#include "ReallocObserver.h"
#include "event/BlockEvent.h"
#include "Initializer.h"

namespace PTrace {

bool ReallocObserver::handle_breakpoint(Breakpoint *breakpoint) {
    Portal *portal = Initializer::get_instance()->get_program_manager()->get_ptrace_portal();
    Word bp = portal->get_register(
#if AESALON_PLATFORM == AESALON_PLATFORM_x86_64
    ASM::Register::RBP
#elif AESALON_PLATFORM == AESALON_PLATFORM_x86
    ASM::Register::EBP
#endif
    );
    Word size, address, new_address, ret_address;
    
    new_address = portal->read_memory(bp-0x08);
    address = portal->read_memory(bp-0x18);
    size = portal->read_memory(bp-0x20);
    ret_address = portal->read_memory(bp-0x28);
    
    Initializer::get_instance()->get_event_queue()->push_event(new Event::BlockEvent(Event::BlockEvent::REALLOC_EVENT, address, size, new_address));
    
    return true;
}

} // namesapce PTrace
