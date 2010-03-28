#ifndef AESALON_GUI_VISUALIZATION_CANVAS_RECT_OBJECT_H
#define AESALON_GUI_VISUALIZATION_CANVAS_RECT_OBJECT_H

#include <QColor>

#include "CanvasObject.h"

class CanvasRectObject : public CanvasObject {
private:
    QColor line_colour, fill_colour;
public:
    CanvasRectObject(const DataRange &bounding_rect, QColor line_colour, QColor fill_colour);
    virtual ~CanvasRectObject();
    
    virtual void paint_onto(QPainter *painter, const CoordinateMapper &mapper);
};

#endif
