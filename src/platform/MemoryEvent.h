#ifndef AESALON_PLATFORM_MEMORY_EVENT_H
#define AESALON_PLATFORM_MEMORY_EVENT_H

#include <string>

#include "MemoryAddress.h"
#include "MemoryBlock.h"
#include "MemoryReference.h"
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
    Misc::SmartPointer<MemoryBlock> block;
public:
    MemoryBlockEvent(memory_block_event_type_e memory_block_event_type, Misc::SmartPointer<MemoryBlock> block) :
        MemoryEvent(MemoryEvent::BLOCK_EVENT), memory_block_event_type(memory_block_event_type), block(block) {}
    virtual ~MemoryBlockEvent() {}
    
    memory_block_event_type_e get_memory_block_event_type() const { return memory_block_event_type; }
    Misc::SmartPointer<MemoryBlock> get_block() const { return block; }
};

class MemoryBlockAllocEvent : public MemoryBlockEvent {
private:
    MemoryAddress address;
    std::size_t size;
public:
    MemoryBlockAllocEvent(MemoryAddress address, std::size_t size) :
        MemoryBlockEvent(MemoryBlockEvent::ALLOC_BLOCK, NULL), address(address), size(size) {}
    virtual ~MemoryBlockAllocEvent() {}
    
    MemoryAddress get_address() const { return address; }
    std::size_t get_size() const { return size; }
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
    Misc::SmartPointer<MemoryReferenceScope> scope;
    Misc::SmartPointer<MemoryBlock> block;
public:
    MemoryReferenceEvent(memory_reference_event_type_e memory_reference_event_type,
        Misc::SmartPointer<MemoryReferenceScope> scope, Misc::SmartPointer<MemoryBlock> block) :
        MemoryEvent(MemoryEvent::REFERENCE_EVENT), memory_reference_event_type(memory_reference_event_type),
        scope(scope), block(block) {}
    virtual ~MemoryReferenceEvent() {}
    
    memory_reference_event_type_e get_memory_reference_event_type() const { return memory_reference_event_type; }
    Misc::SmartPointer<MemoryReferenceScope> get_scope() const { return scope; }
    Misc::SmartPointer<MemoryBlock> get_block() const { return block; }
};

class MemoryReferenceNewEvent : public MemoryReferenceEvent {
public:
    MemoryReferenceNewEvent(Misc::SmartPointer<MemoryReferenceScope> scope, Misc::SmartPointer<MemoryBlock> block) :
        MemoryReferenceEvent(MemoryReferenceEvent::NEW_REFERENCE, scope, block) {}
    virtual ~MemoryReferenceNewEvent();
};

class MemoryReferenceChangedEvent : public MemoryReferenceEvent {
private:
    Misc::SmartPointer<MemoryBlock> new_block;
public:
    MemoryReferenceChangedEvent(Misc::SmartPointer<MemoryReferenceScope> scope,
        Misc::SmartPointer<MemoryBlock> old_block, Misc::SmartPointer<MemoryBlock> new_block) :
        MemoryReferenceEvent(MemoryReferenceEvent::CHANGED_REFERENCE, scope, old_block) {}
    virtual ~MemoryReferenceChangedEvent() {}
    
    Misc::SmartPointer<MemoryBlock> get_new_block() const { return new_block; }
};

class MemoryReferenceRemovedEvent : public MemoryReferenceEvent {
public:
    MemoryReferenceRemovedEvent(Misc::SmartPointer<MemoryReferenceScope> scope,
        Misc::SmartPointer<MemoryBlock> block) :
        MemoryReferenceEvent(MemoryReferenceEvent::REMOVED_REFERENCE, scope, block) {}
    virtual ~MemoryReferenceRemovedEvent() {}
};

} // namespace Platform
} // namespace Aesalon

#endif
