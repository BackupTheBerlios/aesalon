#ifndef AESALON_EXCEPTION_H
#define AESALON_EXCEPTION_H

#include <string>

namespace Aesalon {
namespace Misc {

class Exception {
private:
    std::string message;
public:
    Exception(std::string message) : message(message) {}
    
    std::string get_message() const { return message; }
};

} // namespace Misc
} // namespace Aesalon

#endif
