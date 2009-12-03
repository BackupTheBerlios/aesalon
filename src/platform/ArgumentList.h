#ifndef AESALON_PLATFORM_ARGUMENT_LIST_H
#define AESALON_PLATFORM_ARGUMENT_LIST_H

#include <string>
#include <vector>

namespace Aesalon {
namespace Platform {

class ArgumentList {
public:
    typedef std::vector<std::string> argument_vector_t;
private:
    argument_vector_t argument_vector;
public:
    ArgumentList() {}
    virtual ~ArgumentList() {}
    
    void from_string(std::string string);
    
    void add_argument(std::string argument) { argument_vector.push_back(argument); }
    char **get_as_argv();
};

} // namespace Platform
} // namespace Aesalon

#endif
