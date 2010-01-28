#include <iostream>

#include "Message.h"

namespace Misc {

Message::Message(message_type_e type, std::string string) {
    if(type == WARNING_MESSAGE) {
        std::cout << "{aesalon} " << string << std::endl;
    }
#ifdef AESALON_DEV_BUILD
    else if(type == DEBUG_MESSAGE) {
        std::cout << "{aesalon} " << string << std::endl;
    }
#endif
}

} // namespace Misc
