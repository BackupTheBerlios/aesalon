#ifndef AESALON_MISC_STRING_H
#define AESALON_MISC_STRING_H

#include <string>

namespace Aesalon {
namespace Misc {

class String {
public:
    static bool is_escaped(std::string string, std::string::size_type position);
    static std::string remove_escapes(std::string string);
    static bool begins_with(std::string string, std::string beginning);
};

} // namespace Misc
} // namespace Aesalon

#endif
