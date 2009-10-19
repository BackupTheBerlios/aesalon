#ifndef AESALON_INTERFACE_ARGUMENT_LIST_H
#define AESALON_INTERFACE_ARGUMENT_LIST_H

#include <string>
#include <vector>

namespace Aesalon {
namespace Interface {

class ArgumentList {
public:
    typedef std::vector<std::string> argument_vector_t;
private:
    argument_vector_t argument_vector;
public:
    ArgumentList();
    virtual ~ArgumentList();
    
    void add_argument(std::string argument) { argument_vector.push_back(argument); }
    char **get_as_argv();
};

} // namespace Interface
} // namespace Aesalon

#endif
