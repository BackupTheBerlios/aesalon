#ifndef AESALON_GUI_VISUALIZATION_CANVAS_OBJECT_H
#define AESALON_GUI_VISUALIZATION_CANVAS_OBJECT_H

#include <QPaintDevice>

#include "DataRange.h"
#include "CoordinateMapper.h"

class CanvasObject {
private:
    DataRange bounding_rect;
public:
    CanvasObject(const DataRange &bounding_rect);
    virtual ~CanvasObject();
    
    const DataRange &get_bounding_rect() const { return bounding_rect; }
    
    virtual void paint_onto(QPainter *painter, const CoordinateMapper &mapper) = 0;
};

#endif
