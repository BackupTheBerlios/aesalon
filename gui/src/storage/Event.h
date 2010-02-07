#ifndef AESALON_GUI_STORAGE_EVENT_H
#define AESALON_GUI_STORAGE_EVENT_H

#include "Timestamp.h"
#include "Snapshot.h"

class Event {
private:
    Timestamp timestamp;
public:
    Event() : timestamp() {}
    virtual ~Event() {}
    
    virtual void apply_to(Snapshot *snapshot) = 0;
};

#endif
