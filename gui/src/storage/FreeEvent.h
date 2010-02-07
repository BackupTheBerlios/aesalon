#ifndef AESALON_GUI_STORAGE_FREE_EVENT_H
#define AESALON_GUI_STORAGE_FREE_EVENT_H

#include "Event.h"
#include "MemoryTypes.h"

class FreeEvent : public Event {
private:
    MemoryAddress address;
public:
    FreeEvent(MemoryAddress address) : Event(), address(address) {}
    virtual ~FreeEvent();
    
    virtual void apply_to(Snapshot *snapshot);  
};

#endif
