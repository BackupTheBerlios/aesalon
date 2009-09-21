#ifndef AESALON_INTERFACE_MEMORY_EVENT_H
#define AESALON_INTERFACE_MEMORY_EVENT_H

#include <string>

#include "misc/Event.h"

namespace Aesalon {
namespace Interface {

class MemoryEvent : public Misc::Event {
public:
    enum memory_event_type_e {
        MALLOC_EVENT,
        MEMORY_EVENT_TYPES
    };
private:
    memory_event_type_e memory_type;
    std::string scope;
    std::size_t address;
protected:
    void set_scope(std::string new_scope) { scope = new_scope; }
    void set_address(std::size_t new_address) { address = new_address; }
public:
    MemoryEvent(memory_event_type_e type, std::string scope, std::size_t address) :
        Misc::Event(Misc::Event::MEMORY_EVENT), memory_type(type), scope(scope), address(address) {}

    std::string get_scope() const { return scope; }
    std::size_t get_address() const { return address; }

    memory_event_type_e get_memory_type() const { return memory_type; }
};

class MallocEvent : public MemoryEvent {
private:
    std::size_t size;
public:
    MallocEvent(std::string scope, std::size_t address, std::size_t size) :
        MemoryEvent(MemoryEvent::MALLOC_EVENT, scope, address), size(size) {}
    
    std::size_t get_size() const { return size; }
};

} // namespace Interface
} // namespace Aesalon

#endif
