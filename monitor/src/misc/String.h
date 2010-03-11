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
    
    @file misc/String.h
*/

#ifndef AESALON_MISC_STRING_H
#define AESALON_MISC_STRING_H

#include <string>
#include <ios>

#include "StreamAsString.h"

namespace Misc {

/** Generic string operations class. */
class String {
public:
    /** Returns true if a given position within a string is escaped.
        @param string The string to check.
        @param position The position within @a string to check.
        @return True if the position is escaped, false otherwise.
    */
    static bool is_escaped(std::string string, std::string::size_type position);
    /** Removes the escapes from a string. Turns '\n' into (char)10, for example.
        @param string The string to de-escape.
        @return @a string, de-escaped.
    */
    static std::string remove_escapes(std::string string);
    /** Checks if a string begins with another string.
        @param string The string to check.
        @param beginning The beginning to check for.
        @return True if @a string starts with @a beginning.
    */
    static bool begins_with(std::string string, std::string beginning);
    
    template<typename Type> static void to(std::string string, Type &instance, bool hex = false) {
        std::istringstream parser(string);
        if(hex) parser >> std::hex >> instance;
        else parser >> instance;
    }
    template<typename Type> static std::string from(Type &instance) {
        return Misc::StreamAsString() << instance;
    }
    
    /** Strips whitespace from the beginning of a string.
        @param string The string to strip the whitespace from.
        @return @a string, without whitespace.
    */
    static std::string strip_whitespace(std::string string);
};

} // namespace Misc

#endif
