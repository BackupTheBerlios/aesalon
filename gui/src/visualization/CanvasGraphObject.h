#ifndef AESALON_GUI_VISUALIZATION_CANVAS_GRAPH_OBJECT_H
#define AESALON_GUI_VISUALIZATION_CANVAS_GRAPH_OBJECT_H

#include "CanvasObject.h"

class CanvasGraphObject : public CanvasObject {
private:
    DataPoint data_point;
public:
    CanvasGraphObject(const DataPoint &data_point);
    virtual ~CanvasGraphObject();
    
    virtual void paint_onto(QPainter *painter, const CoordinateMapper &mapper);
};

#endif
