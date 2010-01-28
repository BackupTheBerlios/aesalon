#ifndef AESALON_MONITOR_MESSAGE_H
#define AESALON_MONITOR_MESSAGE_H

#include <string>

namespace Misc {

class Message {
public:
    enum message_type_e {
        WARNING_MESSAGE,
        DEBUG_MESSAGE
    };
    Message(message_type_e type, std::string string);
};

} // namespace Misc

#endif
