#include "CanvasLineObject.h"

CanvasLineObject::CanvasLineObject(const DataPoint &from, const DataPoint &to, QColor line_colour)
    : CanvasObject(DataRange(from, to)), line_colour(line_colour) {

}


CanvasLineObject::~CanvasLineObject() {

}

void CanvasLineObject::paint_onto(QPaintDevice *device, const CoordinateMapper &mapper) {
    
}
