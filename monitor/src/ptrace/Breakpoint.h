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
    
    @file ptrace/Breakpoint.h
*/

#ifndef AESALON_MONITOR_BREAKPOINT_H
#define AESALON_MONITOR_BREAKPOINT_H

#include <vector>

#include "Types.h"
#include "BreakpointObserver.h"

namespace PTrace {

class Breakpoint {
protected:
    typedef std::vector<BreakpointObserver *> observer_list_t;
private:
    Word address;
    Byte original;
    const Byte BREAKPOINT_CHARACTER;
    std::size_t id;
    bool valid;
    
    observer_list_t observer_list;
public:
    Breakpoint(Word address, Byte original);
    virtual ~Breakpoint() {}
    
    std::size_t get_id() const { return id; }
    Byte get_original() const { return original; }
    Word get_address() const { return address; }
    Byte get_breakpoint_character() const { return BREAKPOINT_CHARACTER; }
    
    bool is_valid() const { return valid; }
    void set_valid(bool new_validity) { valid = new_validity; }
    
    void add_observer(BreakpointObserver *observer)
        { observer_list.push_back(observer); set_valid(true); }
    void remove_observer(BreakpointObserver *observer);
    
    void notify();
};

} // namespace PTrace

#endif
