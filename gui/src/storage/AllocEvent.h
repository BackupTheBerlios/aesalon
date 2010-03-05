#ifndef AESALON_GUI_STORAGE_ALLOC_EVENT_H
#define AESALON_GUI_STORAGE_ALLOC_EVENT_H

#include "Event.h"
#include "MemoryTypes.h"

class AllocEvent : public Event {
private:
    MemoryAddress address;
    MemorySize size;
public:
    AllocEvent(const Timestamp &timestamp, MemoryAddress address, MemorySize size) : Event(timestamp), address(address), size(size) {}
    virtual ~AllocEvent() {}
    
    MemoryAddress get_address() const { return address; }
    MemorySize get_size() const { return size; }
    
    virtual void apply_to(Snapshot *snapshot);
};

#endif
