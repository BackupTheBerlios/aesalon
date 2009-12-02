#ifndef AESALON_MISC_STRING_H
#define AESALON_MISC_STRING_H

#include <string>

namespace Aesalon {
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
};

} // namespace Misc
} // namespace Aesalon

#endif
