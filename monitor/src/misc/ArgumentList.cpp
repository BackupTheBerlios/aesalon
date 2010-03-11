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
    
    @file misc/ArgumentList.cpp
*/

#include <cstring>
#include <iostream>
#include "ArgumentList.h"

namespace Misc {

void ArgumentList::from_string(std::string string) {
    /* TODO: support quoted arguments . . . */
    while(string.length()) {
        std::string argument = string.substr(0, string.find(" "));
        if(string.find(" ") != std::string::npos) string.erase(0, string.find(" ")+1);
        else return;
        add_argument(argument);
    }
}

char **ArgumentList::get_as_argv() {
    char **arguments;
    
    arguments = new char *[argument_vector.size()+1];
    
    for(std::size_t x = 0; x < argument_vector.size(); x ++) {
        arguments[x] = new char[std::strlen(argument_vector[x].c_str()) + 1];
        std::strcpy(arguments[x], argument_vector[x].c_str());
    }
    arguments[argument_vector.size()] = 0;
    
    return arguments;
}

} // namespace Misc
