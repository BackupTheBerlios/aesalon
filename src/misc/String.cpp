#include <sstream>

#include "String.h"
#include "Exception.h"

namespace Aesalon {
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
                throw Exception("Trailing backslash in string being un-escaped");
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

} // namespace Misc
} // namespace Aesalon
