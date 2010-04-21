#ifndef AESALON_GUI_VISUALIZATION_CANVAS_TIMELINE_OBJECT_H
#define AESALON_GUI_VISUALIZATION_CANVAS_TIMELINE_OBJECT_H

#include "CanvasObject.h"

class CanvasTimelineObject : public CanvasObject {
private:
    Timestamp timestamp;
    QPen pen;
public:
    CanvasTimelineObject(const Timestamp &timestamp);
    virtual ~CanvasTimelineObject();
    
    virtual void paint_onto(QPainter *painter, const CoordinateMapper &mapper);
    
    void update(const Timestamp &new_timestamp);
};

#endif
