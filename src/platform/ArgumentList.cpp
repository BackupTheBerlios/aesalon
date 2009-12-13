#include <cstring>
#include <iostream>
#include "ArgumentList.h"

namespace Aesalon {
namespace Platform {

void ArgumentList::from_string(std::string string) {
    /* TODO: support quoted arguments . . . */
    while(string.length()) {
        std::string argument = string.substr(0, string.find(" "));
        if(string.find(" ") != std::string::npos) string.erase(0, string.find(" ")+1);
        else return;
        add_argument(argument);
    }
}

char **ArgumentList::get_as_argv() {
    char **arguments;
    
    arguments = new char *[argument_vector.size()+1];
    
    for(std::size_t x = 0; x < argument_vector.size(); x ++) {
        arguments[x] = new char[std::strlen(argument_vector[x].c_str()) + 1];
        std::strcpy(arguments[x], argument_vector[x].c_str());
    }
    arguments[argument_vector.size()] = 0;
    
    return arguments;
}

} // namespace Platform
} // namespace Aesalon
