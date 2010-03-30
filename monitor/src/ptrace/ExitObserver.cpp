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
    
    @file ptrace/ExitObserver.cpp
*/

#include <signal.h>

#include "ExitObserver.h"
#include "Initializer.h"

namespace PTrace {

bool ExitObserver::handle_signal(int signal, int status) {
    if(signal == SIGKILL || signal == SIGTERM || signal == -1) {
        Initializer::get_instance()->get_program_manager()->set_running(false);
        return true;
    }
    return false;
}

} // namespace PTrace
