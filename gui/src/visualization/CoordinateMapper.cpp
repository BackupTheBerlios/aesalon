#include "CoordinateMapper.h"

CoordinateMapper::CoordinateMapper(QSize surface_size, DataRange data_range)
    : surface_size(surface_size), data_range(data_range) {

}

CoordinateMapper::~CoordinateMapper() {

}

QPointF CoordinateMapper::map_to(const DataPoint &point) const {
    /* NOTE: this is the unoptimized version . . . */
    qint64 total_time = data_range.get_begin().get_time_element().ns_until(data_range.get_end().get_time_element());
    qreal time_percentage = data_range.get_begin().get_time_element().ns_until(point.get_time_element()) / total_time;
    /* Bind it to slightly more than the total size of the surface . . . */
    time_percentage = qBound(-0.01, 1.01, time_percentage);
    
    qreal total_data = data_range.get_end().get_data_element() - data_range.get_begin().get_data_element();
    qreal data_percentage = (point.get_data_element() - data_range.get_begin().get_data_element()) / total_data;
    /* Bind it to slightly more than the total size of the surface, and reverse it
        (data zero is at the bottom of the image, not the top) */
    data_percentage = 1.0 - qBound(-0.01, 1.01, data_percentage);
    
    return QPointF(surface_size.width() * time_percentage, surface_size.height() * data_percentage);
}

QRectF CoordinateMapper::map_to(const DataRange &rect) const {
    return QRectF(map_to(rect.get_begin()), map_to(rect.get_end()));
}

DataPoint CoordinateMapper::map_to(const QPointF &point) const {
    qint64 total_time = data_range.get_begin().get_time_element().ns_until(data_range.get_end().get_time_element());
    qreal time_percentage = point.x() / surface_size.width();
    
    qreal total_data = data_range.get_end().get_data_element() - data_range.get_begin().get_data_element();
    qreal data_percentage = 1.0 - point.y() / surface_size.height();
    
    Timestamp temporary = data_range.get_begin().get_time_element();
    temporary.add_ns(total_time * time_percentage);
    return DataPoint(temporary, (total_data * data_percentage) + data_range.get_begin().get_data_element());
}

DataRange CoordinateMapper::map_to(const QRectF &rect) const {
    return DataRange(map_to(rect.topLeft()), map_to(rect.bottomRight()));
}
