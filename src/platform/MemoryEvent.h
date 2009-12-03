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
        REFERENCE_EVENT
    };
private:
    memory_event_type_e memory_event_type;
public:
    MemoryEvent(memory_event_type_e memory_event_type)
        : Event(MEMORY_EVENT), memory_event_type(memory_event_type) {}
    virtual ~MemoryEvent() {}
    
    memory_event_type_e get_memory_event_type() const { return memory_event_type; }
    
    virtual std::string serialize() const = 0;
};

class MemoryBlockEvent : public MemoryEvent {
public:
    enum memory_block_event_type_e {
        ALLOC_EVENT,
        RESIZE_EVENT,
        FREE_EVENT
    };
private:
    memory_block_event_type_e memory_block_event_type;
public:
    MemoryBlockEvent(memory_block_event_type_e memory_block_event_type)
        : MemoryEvent(MemoryEvent::BLOCK_EVENT), memory_block_event_type(memory_block_event_type) {}
    virtual ~MemoryBlockEvent() {}
    
    memory_block_event_type_e get_memory_block_event_type() const { return memory_block_event_type; }
};

class MemoryBlockAllocEvent : public MemoryBlockEvent {
private:
    MemoryAddress block_address;
    std::size_t block_size;
public:
    MemoryBlockAllocEvent(MemoryAddress block_address, std::size_t block_size)
        : MemoryBlockEvent(MemoryBlockEvent::ALLOC_EVENT), block_address(block_address), block_size(block_size) {}
    virtual ~MemoryBlockAllocEvent() {}
    
    MemoryAddress get_block_address() const { return block_address; }
    std::size_t get_block_size() const { return block_size; }
    
    virtual std::string serialize() const;
};

class MemoryBlockResizeEvent : public MemoryBlockEvent {
private:
    MemoryAddress old_block_address;
    MemoryAddress new_block_address;
    std::size_t new_block_size;
public:
    MemoryBlockResizeEvent(MemoryAddress old_block_address, MemoryAddress new_block_address, std::size_t block_size)
         : MemoryBlockEvent(MemoryBlockEvent::ALLOC_EVENT), old_block_address(old_block_address),
         new_block_address(new_block_address), new_block_size(new_block_size) {}
    virtual ~MemoryBlockResizeEvent() {}
    
    MemoryAddress get_old_block_address() const { return old_block_address; }
    MemoryAddress get_new_block_address() const { return new_block_address; }
    std::size_t get_new_block_size() const { return new_block_size; }
    
    virtual std::string serialize() const;
};

class MemoryBlockFreeEvent : public MemoryBlockEvent {
private:
    MemoryAddress block_address;
public:
    MemoryBlockFreeEvent(MemoryAddress block_address)
        : MemoryBlockEvent(MemoryBlockEvent::FREE_EVENT), block_address(block_address) {}
    virtual ~MemoryBlockFreeEvent() {}
    
    MemoryAddress get_block_address() const { return block_address; }
    
    virtual std::string serialize() const;
};



} // namespace Platform
} // namespace Aesalon

#endif
