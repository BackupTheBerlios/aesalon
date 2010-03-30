#include <QThreadStorage>
#include "CanvasGraphObject.h"

CanvasGraphObject::CanvasGraphObject(const DataPoint &data_point): CanvasObject(DataRange(data_point, data_point)), data_point(data_point) {
    
}

CanvasGraphObject::~CanvasGraphObject() {

}

void CanvasGraphObject::paint_onto(QPainter *painter, const CoordinateMapper &mapper) {
    QPointF point = mapper.map_to(data_point);
    QPointF point2 = mapper.map_to(DataPoint(data_point.get_time_element(), 0.0));
    painter->drawLine(point, point2);
}
