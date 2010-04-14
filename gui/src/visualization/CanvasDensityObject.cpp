#include "CanvasDensityObject.h"
#include <QPaintEngine>

CanvasDensityObject::CanvasDensityObject(const DataRange &rectangle, QColor line_colour)
    : CanvasObject(rectangle), line_colour(line_colour) {
    
    uint hash = qHash(rectangle.get_begin());
    
    /* Take the first nine bits, in groups of three bits each.
        Take the value of the two bits, multiply by 32 to find the colour.
        
        Masks:
            0x01 + 0x02 + 0x04 = 0x07
            0x08 + 0x10 + 0x20 = 0x38
            0x40 + 0x80 + 0x100 = 0x1c0
    */
    
    /* NOTE: the +1 on the red is to ensure a block is never totally black. */
    fill_colour = qRgb((((hash & 0x07) + 1) << 5), ((hash & 0x38) >> 3) << 5, ((hash & 0x1c0) >> 6) << 5);
}

CanvasDensityObject::~CanvasDensityObject() {
    
}

void CanvasDensityObject::paint_onto(QPainter *painter, const CoordinateMapper &mapper) {
    QPen pen(Qt::SolidLine);
    pen.setColor(line_colour);
    painter->setPen(pen);
    painter->setBrush(QBrush(fill_colour));
    QRectF rect = mapper.map_to(get_bounding_rect());
    /*qDebug("CanvasDensityObject::paint_onto(): rect is (%f, %f), (%f, %f)", rect.left(), rect.top(), rect.right(), rect.bottom());
    qDebug("painter->size() is (%i, %i)", painter->device()->width(), painter->device()->height());*/
    painter->drawRect(rect);
}
