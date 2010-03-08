#ifndef AESALON_GUI_STORAGE_EVENT_H
#define AESALON_GUI_STORAGE_EVENT_H

#include "Timestamp.h"
#include "Snapshot.h"

class EventVisitor;

class Event {
private:
    Timestamp timestamp;
public:
    Event(const Timestamp &timestamp) : timestamp(timestamp) {}
    virtual ~Event() {}
    
    const Timestamp &get_timestamp() const { return timestamp; }
    
    virtual void apply_to(Snapshot *snapshot) = 0;
    
    virtual void accept(EventVisitor &visitor) = 0;
};

#endif
