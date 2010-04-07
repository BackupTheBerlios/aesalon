#include "CanvasRectObject.h"
#include <QPaintEngine>

CanvasRectObject::CanvasRectObject(const DataRange &rectangle, QColor line_colour, QColor fill_colour)
    : CanvasObject(rectangle), line_colour(line_colour), fill_colour(fill_colour) {
    
    /*this->fill_colour = qRgb(qrand() % 256, qrand() % 256, qrand() % 256);*/
}

CanvasRectObject::~CanvasRectObject() {
    
}

void CanvasRectObject::paint_onto(QPainter *painter, const CoordinateMapper &mapper) {
    QPen pen(Qt::SolidLine);
    pen.setColor(line_colour);
    painter->setPen(pen);
    painter->setBrush(QBrush(fill_colour));
    QRectF rect = mapper.map_to(get_bounding_rect());
    qDebug("CanvasRectObject::paint_onto(): rect is (%f, %f), (%f, %f)", rect.left(), rect.top(), rect.right(), rect.bottom());
    qDebug("painter->size() is (%i, %i)", painter->device()->width(), painter->device()->height());
    painter->drawRect(rect);
}
