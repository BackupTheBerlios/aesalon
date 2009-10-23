#ifndef AESALON_PLATFORM_MEMORY_EVENT_H
#define AESALON_PLATFORM_MEMORY_EVENT_H

#include <string>

#include "misc/Event.h"

namespace Aesalon {
namespace Platform {

class MemoryEvent : public Misc::Event {
public:
    enum memory_event_type_e {
        BLOCK_EVENT,
        REFERENCE_EVENT,
        MEMORY_EVENT_TYPES
    };
private:
    memory_event_type_e memory_event_type;
public:
    MemoryEvent(memory_event_type_e memory_event_type) :
        Misc::Event(Misc::Event::MEMORY_EVENT), memory_event_type(memory_event_type) {}
    virtual ~MemoryEvent() {}
    
    memory_event_type_e get_memory_event_type() const { return memory_event_type; }
    
    virtual std::string serialize() = 0;
};

class MemoryBlockEvent : public MemoryEvent {
public:
    enum memory_block_event_type_e {
        ALLOC_BLOCK,
        RESIZED_BLOCK,
        FREE_BLOCK,
        MEMORY_BLOCK_EVENT_TYPES
    };
private:
    memory_block_event_type_e memory_block_event_type;
public:
    MemoryBlockEvent(memory_block_event_type_e memory_block_event_type) :
        MemoryEvent(MemoryEvent::BLOCK_EVENT), memory_block_event_type(memory_block_event_type) {}
    virtual ~MemoryBlockEvent() {}
    
    memory_block_event_type_e get_memory_block_event_type() const { return memory_block_event_type; }
};

class MemoryReferenceEvent : public MemoryEvent {
public:
    enum memory_reference_event_type_e {
        NEW_REFERENCE,
        CHANGED_REFERENCE,
        REMOVED_REFERENCE,
        MEMORY_REFERENCE_EVENT_TYPES
    };
private:
    memory_reference_event_type_e memory_reference_event_type;
public:
    MemoryReferenceEvent(memory_reference_event_type_e memory_reference_event_type) :
        MemoryEvent(MemoryEvent::REFERENCE_EVENT), memory_reference_event_type(memory_reference_event_type) {}
    
    memory_reference_event_type_e get_memory_reference_event_type() const { return memory_reference_event_type; }
};



} // namespace Platform
} // namespace Aesalon

#endif
