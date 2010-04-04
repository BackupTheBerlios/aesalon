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
    QRectF rect = mapper.map_to(get_bounding_rect());
    qDebug("Drawing CanvasRectObject: rect is (%f,%f) to (%f,%f)", rect.left(), rect.top(), rect.right(), rect.bottom());
    painter->drawRect(rect);
}
