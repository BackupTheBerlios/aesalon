#ifndef AESALON_GUI_VISUALIZATION_FILTER_H
#define AESALON_GUI_VISUALIZATION_FILTER_H

#include "storage/Event.h"

class Filter {
private:
    Filter *child;
    Filter *next;
public:
    Filter();
    ~Filter();
    
    Filter *get_next() const { return next; }
    void set_next(Filter *new_next) { next = new_next; }
    
    Filter *get_child() const { return child; }
    void set_child(Filter *new_child) { child = new_child; }
    void add_child(Filter *new_child);
    
    virtual bool passes_filter(Event *event);
};

Q_DECLARE_METATYPE(Filter)

#endif
