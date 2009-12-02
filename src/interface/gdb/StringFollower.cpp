#include <sstream>

#include "StringFollower.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

std::string StringFollower::follow(std::string path) {
    std::stringstream stream;
    stream << path;
    
    std::string token;
    
    while(stream >> token) {
        if(token[0] == '[') { /* list element */
            if(token[token.length()-1] != ']') {
                throw StringFollowerInvalidPathException("Expected terminating ']' at end of token.");
            }
            std::istringstream number_stream(token.substr(1, token.length()-2));
            int number;
            number_stream >> number;
            
            data = data.to<ParseList>()->get_element(number);
        }
        else if(token[0] == '\'') { /* tuple element */
            if(token[token.length()-1] != '\'') {
                throw StringFollowerInvalidPathException("Expected terminating \' at end of token.");
            }
            std::string key = token.substr(1, token.length()-2);
            data = data.to<ParseTuple>()->get_element(key);
        }
        else if(token[0] == '\"') { /* tuple element */
            if(token[token.length()-1] != '\"') {
                throw StringFollowerInvalidPathException("Expected terminating \" at end of token.");
            }
            std::string key = token.substr(1, token.length()-2);
            data = data.to<ParseTuple>()->get_element(key);
        }
        else if(token == "lhs") { /* left side of a result */
            data = new ParseString(data.to<ParseResult>()->get_name());
        }
        else if(token == "rhs") { /* right side of a result */
            data = data.to<ParseResult>()->get_value();
        }
    }
    
    return data.to<ParseString>()->get_data();
}

} // namespace GDB
} // namespace Interface
} // namespace Aesalon