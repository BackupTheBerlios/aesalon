#ifndef AESALON_MISC_EVENT_H
#define AESALON_MISC_EVENT_H

#include <string>

namespace Event {

/** Event class. Note that until overloaded, this base class is rather
    useless. */
class BasicEvent {
public:
    /** Event type enum, used for dynamic_cast<>ing later. */
    enum event_type_e {
        BLOCK_EVENT
    };
private:
    /** Current event type. */
    event_type_e type;
public:
    /** Constructor, takes a type. All other stored data is in the derived
        types. */
    BasicEvent(event_type_e type) : type(type) {}
    virtual ~BasicEvent() {}
    
    /** Returns the type of the current event.
        @return The type of the current event.
    */
    event_type_e get_type() const { return type; }
    
    virtual std::string serialize();
};

} // namespace Event

#endif
