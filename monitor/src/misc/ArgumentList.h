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
    
    @file misc/ArgumentList.h
*/

#ifndef AESALON_MISC_ARGUMENT_LIST_H
#define AESALON_MISC_ARGUMENT_LIST_H

#include <string>
#include <vector>

namespace Misc {

class ArgumentList {
public:
    typedef std::vector<std::string> argument_vector_t;
private:
    argument_vector_t argument_vector;
public:
    ArgumentList() {}
    virtual ~ArgumentList() {}
    
    void from_string(std::string string);
    
    void add_argument(std::string argument) { argument_vector.push_back(argument); }
    char **get_as_argv();
    
    std::string get_argument(argument_vector_t::size_type which) {
        return argument_vector[which];
    }
};

} // namespace Misc

#endif
