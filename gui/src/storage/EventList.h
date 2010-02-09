#ifndef AESALON_GUI_STORAGE_EVENT_LIST_H
#define AESALON_GUI_STORAGE_EVENT_LIST_H

#include <QList>

class Event;

class EventList {
private:
    QList<Event *> events;
public:
    EventList() {}
    ~EventList() {}
    
    void add_event(Event *event) {
        events.append(event);
    }
    QList<Event *> get_event_list() const { return events; }
};

#endif
