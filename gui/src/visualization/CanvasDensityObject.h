#ifndef AESALON_GUI_VISUALIZATION_CANVAS_DENSITY_OBJECT_H
#define AESALON_GUI_VISUALIZATION_CANVAS_DENSITY_OBJECT_H

#include <QColor>

#include "CanvasObject.h"

class CanvasDensityObject : public CanvasObject {
private:
    QColor line_colour, fill_colour;
public:
    CanvasDensityObject(const DataRange &bounding_rect, QColor line_colour);
    virtual ~CanvasDensityObject();
    
    virtual void paint_onto(QPainter *painter, const CoordinateMapper &mapper);
};

#endif
