#ifndef AESALON_PLATFORM_MEMORY_EVENT_H
#define AESALON_PLATFORM_MEMORY_EVENT_H

#include <string>

#include "MemoryAddress.h"
#include "MemoryBlock.h"
#include "MemoryReference.h"
#include "Event.h"

namespace Aesalon {
namespace Platform {

class MemoryEvent : public Event {
public:
    enum memory_event_type_e {
        BLOCK_EVENT,
        REFERENCE_EVENT,
        MEMORY_EVENT_TYPES
    };
private:
    memory_event_type_e memory_event_type;
    Misc::SmartPointer<MemoryBlock> block;
public:
    MemoryEvent(memory_event_type_e memory_event_type, Misc::SmartPointer<MemoryBlock> block) :
        Event(Event::MEMORY_EVENT), memory_event_type(memory_event_type), block(block) {}
    virtual ~MemoryEvent() {}
    
    memory_event_type_e get_memory_event_type() const { return memory_event_type; }
    Misc::SmartPointer<MemoryBlock> get_block() const { return block; }
    
    virtual std::string serialize() const = 0;
    static MemoryEvent *deserialize(std::string data);
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
    
    MemoryAddress address;
    std::size_t size;
public:
    MemoryBlockEvent(memory_block_event_type_e memory_block_event_type,
        Misc::SmartPointer<MemoryBlock> block, MemoryAddress address, std::size_t size) :
        MemoryEvent(MemoryEvent::BLOCK_EVENT, block),
        memory_block_event_type(memory_block_event_type), address(address), size(size) {}
    virtual ~MemoryBlockEvent() {}
    
    memory_block_event_type_e get_memory_block_event_type() const { return memory_block_event_type; }
    MemoryAddress get_address() const { return address; }
    std::size_t get_size() const { return size; }
};

class MemoryBlockAllocEvent : public MemoryBlockEvent {
public:
    MemoryBlockAllocEvent(MemoryAddress address, std::size_t size) :
        MemoryBlockEvent(MemoryBlockEvent::ALLOC_BLOCK, NULL, address, size)  {}
    virtual ~MemoryBlockAllocEvent() {}
    
    virtual std::string serialize() const;
};

class MemoryBlockResizedEvent : public MemoryBlockEvent {
public:
    MemoryBlockResizedEvent(Misc::SmartPointer<MemoryBlock> block,
        MemoryAddress new_address, std::size_t new_size) :
        MemoryBlockEvent(MemoryBlockEvent::RESIZED_BLOCK, block, new_address,
        new_size) {}
    virtual ~MemoryBlockResizedEvent() {}
    
    virtual std::string serialize() const;
};

class MemoryBlockFreeEvent : public MemoryBlockEvent {
public:
    MemoryBlockFreeEvent(Misc::SmartPointer<MemoryBlock> block) : 
        MemoryBlockEvent(MemoryBlockEvent::FREE_BLOCK, block, 0, 0) {}
    virtual ~MemoryBlockFreeEvent() {}
    
    virtual std::string serialize() const;
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
public:
    MemoryReferenceEvent(memory_reference_event_type_e memory_reference_event_type,
        Misc::SmartPointer<MemoryReferenceScope> scope, Misc::SmartPointer<MemoryBlock> block) :
        MemoryEvent(MemoryEvent::REFERENCE_EVENT, block), memory_reference_event_type(memory_reference_event_type),
        scope(scope) {}
    virtual ~MemoryReferenceEvent() {}
    
    memory_reference_event_type_e get_memory_reference_event_type() const { return memory_reference_event_type; }
    Misc::SmartPointer<MemoryReferenceScope> get_scope() const { return scope; }
};

class MemoryReferenceNewEvent : public MemoryReferenceEvent {
public:
    MemoryReferenceNewEvent(Misc::SmartPointer<MemoryReferenceScope> scope, Misc::SmartPointer<MemoryBlock> block) :
        MemoryReferenceEvent(MemoryReferenceEvent::NEW_REFERENCE, scope, block) {}
    virtual ~MemoryReferenceNewEvent() {}
    
    virtual std::string serialize() const;
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
    
    virtual std::string serialize() const;
};

class MemoryReferenceRemovedEvent : public MemoryReferenceEvent {
public:
    MemoryReferenceRemovedEvent(Misc::SmartPointer<MemoryReferenceScope> scope,
        Misc::SmartPointer<MemoryBlock> block) :
        MemoryReferenceEvent(MemoryReferenceEvent::REMOVED_REFERENCE, scope, block) {}
    virtual ~MemoryReferenceRemovedEvent() {}
    
    virtual std::string serialize() const;
};

} // namespace Platform
} // namespace Aesalon

#endif
