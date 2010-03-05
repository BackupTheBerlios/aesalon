#ifndef AESALON_GUI_STORAGE_FREE_EVENT_H
#define AESALON_GUI_STORAGE_FREE_EVENT_H

#include "Event.h"
#include "MemoryTypes.h"

class FreeEvent : public Event {
private:
    MemoryAddress address;
public:
    FreeEvent(const Timestamp &timestamp, MemoryAddress address) : Event(timestamp), address(address) {}
    virtual ~FreeEvent() {}
    
    virtual void apply_to(Snapshot *snapshot);  
};

#endif
