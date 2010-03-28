#ifndef AESALON_GUI_VISUALIZATION_COORDINATE_MAPPER_H
#define AESALON_GUI_VISUALIZATION_COORDINATE_MAPPER_H

#include <QPoint>
#include <QRect>

#include "DataPoint.h"
#include "DataRange.h"

class CoordinateMapper {
private:
    QSize surface_size;
    DataRange data_range;
public:
    CoordinateMapper(QSize surface_size, DataRange data_range);
    virtual ~CoordinateMapper();
    
    QSize get_surface_size() const { return surface_size; }
    void set_surfce_size(QSize new_size) { surface_size = new_size; }
    
    QPointF map_to(const DataPoint &point) const;
    QRectF map_to(const DataRange &rect) const;
    DataPoint map_to(const QPointF &point) const;
    DataRange map_to(const QRectF &rect) const;
};

#endif
