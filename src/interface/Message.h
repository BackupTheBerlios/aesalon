#ifndef AESALON_INTERFACE_MESSAGE_H
#define AESALON_INTERFACE_MESSAGE_H

#include <string>

namespace Aesalon {
namespace Interface {

class Message {
public:
    enum message_type_e {
        WARNING_MESSAGE,
        DEBUG_MESSAGE
    };
    Message(message_type_e type, std::string string);
};

} // namespace Interface
} // namespace Aesalon

#endif
