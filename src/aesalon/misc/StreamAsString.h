#ifndef AESALON_MISC_STREAM_AS_STRING_H
#define AESALON_MISC_STREAM_AS_STRING_H

#include <string>
#include <sstream>

namespace Aesalon {
namespace Misc {

/** StreamAsString class.

    Allows for nice inline stringstreams, such as
Exception(StreamAsString() << "Exception occured, line " << __line__);
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
} // namespace Aesalon

#endif
