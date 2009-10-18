#include <cstring>
#include "ArgumentList.h"

namespace Aesalon {
namespace Interface {

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

} // namespace Interface
} // namespace Aesalon
