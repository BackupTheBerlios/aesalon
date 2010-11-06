/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/StreamAsString.h

*/

#ifndef AesalonCommon_StreamAsString_H
#define AesalonCommon_StreamAsString_H

#include <sstream>

namespace Common {

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

} // namespace Common

#endif
