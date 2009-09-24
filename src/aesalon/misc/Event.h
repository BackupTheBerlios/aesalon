#ifndef AESALON_MISC_EVENT_H
#define AESALON_MISC_EVENT_H

namespace Aesalon {
namespace Misc {

/** Event class. Note that until overloaded, this base class is rather
    useless. */
class Event {
public:
    /** Event type enum, used for dynamic_cast<>ing later. */
    enum event_type_e {
        MEMORY_EVENT,
        EVENT_TYPES
    };
private:
    /** Current event type. */
    event_type_e type;
public:
    /** Constructor, takes a type. All other stored data is in the derived
        types. */
    Event(event_type_e type) : type(type) {}
    
    /** Returns the type of the current event.
        @return The type of the current event.
    */
    event_type_e get_type() const { return type; }
};

} // namespace Misc
} // namespace Aesalon

#endif
