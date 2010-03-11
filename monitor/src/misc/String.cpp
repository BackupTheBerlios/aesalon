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
    
    @file misc/String.cpp
*/

#include <sstream>

#include "String.h"
#include "exception/BasicException.h"

namespace Misc {

bool String::is_escaped(std::string string, std::string::size_type position) {
    int backslash_count = 0;
    
    while(--position != std::string::size_type(-1)) {
        if(string[position] == '\\') backslash_count ++;
        else break;
    }
    
    if((backslash_count % 2) == 0) return false;
    return true;
}

std::string String::remove_escapes(std::string string) {
    std::string result;
    
    for(std::string::iterator i = string.begin(); i != string.end(); i ++) {
        if(*i == '\\') {
            i ++;
            if(i == string.end()) {
                throw Exception::BasicException("Trailing backslash in string being un-escaped");
            }
            
            switch(*i) {
                case 'n':
                    result += '\n';
                    break;
                case 't':
                    result += '\t';
                    break;
                default:
                    result += *i;
                    break;
            }
        }
        else result += *i;
    }
    
    return result;
}

bool String::begins_with(std::string string, std::string beginning) {
    return string.substr(0, beginning.length()) == beginning;
}

std::string String::strip_whitespace(std::string string) {
    while(std::isspace(string[0]) && string.length()) string.erase(0, 1);
    return string;
}

} // namespace Misc
