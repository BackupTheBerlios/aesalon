#ifndef AESALON_GUI_STORAGE_EVENT_VISITOR_H
#define AESALON_GUI_STORAGE_EVENT_VISITOR_H

class AllocEvent;
class FreeEvent;

class EventVisitor {
public:
    EventVisitor() {}
    virtual ~EventVisitor() {}
    
    virtual void visit(AllocEvent *event) = 0;
    virtual void visit(FreeEvent *event) = 0;
};

#endif
