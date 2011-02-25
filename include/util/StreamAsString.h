/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/util/StreamAsString.h
*/

#ifndef AesalonUtil_StreamAsString_H
#define AesalonUtil_StreamAsString_H

#include <sstream>

namespace Util {

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

} // namespace Util

/* Keep the above class working with QStrings (if in a program that uses Qt) . . . */
#ifdef QT_VERSION
std::ostream &operator<<(std::ostream &stream, const QString &string);
#endif


#endif

