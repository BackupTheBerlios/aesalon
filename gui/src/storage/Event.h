#ifndef AESALON_GUI_STORAGE_EVENT_H
#define AESALON_GUI_STORAGE_EVENT_H

#include "Timestamp.h"

class Event {
private:
    Timestamp timestamp;
public:
    Event();
    virtual ~Event() {}
};

#endif
