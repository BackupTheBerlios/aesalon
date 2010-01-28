#ifndef AESALON_EXCEPTION_H
#define AESALON_EXCEPTION_H

#include <string>

namespace Aesalon {
namespace Misc {

/** Basic exception class, stores a message. Constuctor can be overloaded in
    derived classes for more specific messages. */
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
