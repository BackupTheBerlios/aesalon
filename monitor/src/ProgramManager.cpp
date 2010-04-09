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
    
    @file ProgramManager.cpp
    This file contains the implementation of the ProgramManager class.
*/

#include <iostream>
#include <signal.h>
#include "ProgramManager.h"
#include "Initializer.h"

ProgramManager::ProgramManager(Misc::ArgumentList *argument_list)
    : argument_list(argument_list), running(false), ptrace_portal(NULL) {
    
#ifdef USE_OVERLOAD
    overload_parser = NULL;
#endif
}

ProgramManager::~ProgramManager() {
    if(ptrace_portal) delete ptrace_portal;
    delete argument_list;
}

void ProgramManager::execute() {
    running = true;
    ptrace_portal = new PTrace::Portal(get_argument_list());
#ifdef USE_OVERLOAD
    overload_parser = new OverloadParser(ptrace_portal->get_pipe_fd());
#endif
}

void ProgramManager::wait() {
    ptrace_portal->handle_signal();
}

void ProgramManager::setup() {
    
}

void ProgramManager::process_backlog() {
#ifdef USE_OVERLOAD
    delete overload_parser;
#endif
}