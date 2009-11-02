#ifndef AESALON_MISC_STRING_ESCAPER_H
#define AESALON_MISC_STRING_ESCAPER_H

#include <string>

namespace Aesalon {
namespace Misc {

class StringEscaper {
public:
    static bool is_escaped(std::string string, std::string::size_type position);
    static std::string remove_escapes(std::string string);
};

} // namespace Misc
} // namespace Aesalon

#endif
