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
    
    @file misc/StreamAsString.h
*/

#ifndef AESALON_MISC_STREAM_AS_STRING_H
#define AESALON_MISC_STREAM_AS_STRING_H

#include <string>
#include <sstream>

namespace Misc {

/** StreamAsString class.

    Allows for nice inline stringstreams, such as BasicException(StreamAsString()
        << "Exception occured, line " << __line__);
*/
class StreamAsString {
public:
    /** Automatic conversion to a std::string.
        @return The internal stringstream, converted to a std::string.
    */
    operator std::string() const {
        return get_stream().str();
    }
    /** Add something onto the internal std::stringstream.
        @param data The data to add.
        @return The new stringstream, with the data added.
    */
    template<typename Type>
    StreamAsString &operator<<(const Type &data) {
        get_stream() << data;
        
        return *this;
    }
protected:
    /** Get the internal stringstream.
        @return The internal stringstream, in non-const form.
    */
    std::ostringstream &get_stream() { return stream; }
    /** Get the internal stringstream.
        @return The internal stringstream, in const form.
    */
    const std::ostringstream &get_stream() const { return stream; }
private:
    /** The internal stringstream. */
    std::ostringstream stream;
};

} // namespace Misc

#endif
