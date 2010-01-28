#ifndef AESALON_MISC_ARGUMENT_LIST_H
#define AESALON_MISC_ARGUMENT_LIST_H

#include <string>
#include <vector>

namespace Misc {

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
    
    std::string get_argument(argument_vector_t::size_type which) {
        return argument_vector[which];
    }
};

} // namespace Misc

#endif
