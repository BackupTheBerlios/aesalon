#include "CanvasLineObject.h"

CanvasLineObject::CanvasLineObject(const DataPoint &from, const DataPoint &to, QColor line_colour)
    : CanvasObject(DataRange(from, to)), line_colour(line_colour) {

}


CanvasLineObject::~CanvasLineObject() {

}

void CanvasLineObject::paint_onto(QPainter *painter, const CoordinateMapper &mapper) {
    painter->setPen(line_colour);
    painter->drawLine(mapper.map_to(get_bounding_rect().get_begin()), mapper.map_to(get_bounding_rect().get_end()));
}
