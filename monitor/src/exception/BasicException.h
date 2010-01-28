#ifndef AESALON_EXCEPTION_H
#define AESALON_EXCEPTION_H

#include <string>

namespace Exception {

/** Basic exception class, stores a message. Constuctor can be overloaded in
    derived classes for more specific messages. */
class BasicException {
private:
    std::string message;
public:
    BasicException(std::string message) : message(message) {}
    
    std::string get_message() const { return message; }
};

} // namespace Exception

#endif
