#ifndef AESALON_MISC_EVENT_H
#define AESALON_MISC_EVENT_H

namespace Aesalon {
namespace Misc {

class Event {
public:
    enum event_type_e {
        MEMORY_EVENT,
        EVENT_TYPES
    };
private:
    event_type_e type;
public:
    Event(event_type_e type) : type(type) {}
    
    event_type_e get_type() const { return type; }
};

} // namespace Misc
} // namespace Aesalon

#endif
