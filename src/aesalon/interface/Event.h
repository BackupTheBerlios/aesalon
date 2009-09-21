#ifndef AESALON_INTERFACE_EVENT_H
#define AESALON_INTERFACE_EVENT_H

namespace Aesalon {
namespace Interface {

class Event {
public:
    enum event_type_e {
        EVENT_TYPES
    };
private:
    event_type_e type;
public:
    Event(event_type_e type) : type(type) {}
    
    event_type_e get_type() const { return type; }
};

} // namespace Interface
} // namespace Aesalon

#endif
