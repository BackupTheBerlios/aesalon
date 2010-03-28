#include "CanvasRectObject.h"
#include <QPaintEngine>

CanvasRectObject::CanvasRectObject(const DataRange &rectangle, QColor line_colour, QColor fill_colour)
    : CanvasObject(rectangle), line_colour(line_colour), fill_colour(fill_colour) {

}

CanvasRectObject::~CanvasRectObject() {
    
}

void CanvasRectObject::paint_onto(QPainter *painter, const CoordinateMapper &mapper) {
    painter->setPen(line_colour);
    painter->setBrush(QBrush(fill_colour));
    painter->drawRect(mapper.map_to(get_bounding_rect()));
}
