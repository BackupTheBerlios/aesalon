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
    
    @file ptrace/BreakpointObserver.h
*/

#ifndef AESALON_MONITOR_PTRACE_BREAKPOINT_OBSERVER_H
#define AESALON_MONITOR_PTRACE_BREAKPOINT_OBSERVER_H

namespace PTrace {

class Breakpoint;

class BreakpointObserver {
public:
    BreakpointObserver() {}
    virtual ~BreakpointObserver() {}

    /** Process a breakpoint event.
        @param breakpoint The breakpoint that triggered itself.
        @return True if the observer is still valid.
    */
    virtual bool handle_breakpoint(Breakpoint *breakpoint) = 0;
};

} // namespace PTrace

#endif
