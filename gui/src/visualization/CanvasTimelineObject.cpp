#include "CanvasTimelineObject.h"

CanvasTimelineObject::CanvasTimelineObject(const Timestamp &timestamp)
    : CanvasObject(DataRange(timestamp, 0, timestamp, 0)), timestamp(timestamp) {
    pen = QPen(Qt::SolidLine);
    pen.setColor(qRgb(255, 0, 0));
    pen.setWidth(1);
}

CanvasTimelineObject::~CanvasTimelineObject() {

}

void CanvasTimelineObject::paint_onto(QPainter *painter, const CoordinateMapper &mapper) {
    painter->setPen(pen);
    QPointF point = mapper.map_to(get_bounding_rect().get_end());
    painter->drawLine(point.x(), 0, point.x(), (painter->device()->height()-1));
}

void CanvasTimelineObject::update(const Timestamp &new_timestamp) {
    get_bounding_rect().get_end().set_time_element(new_timestamp);
}
