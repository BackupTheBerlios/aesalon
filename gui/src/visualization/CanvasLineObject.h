#ifndef AESALON_GUI_VISUALIZATION_CANVAS_LINE_OBJECT_H
#define AESALON_GUI_VISUALIZATION_CANVAS_LINE_OBJECT_H

#include <QColor>

#include "CanvasObject.h"

class CanvasLineObject : public CanvasObject {
private:
    QColor line_colour;
public:
    CanvasLineObject(const DataPoint &from, const DataPoint &to, QColor line_colour);
    virtual ~CanvasLineObject();
    
    virtual void paint_onto(QPaintDevice *device, const CoordinateMapper &mapper);
};

#endif
