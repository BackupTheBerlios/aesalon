#ifndef AESALON_GUI_VISUALIZATION_DATA_POINT_H
#define AESALON_GUI_VISUALIZATION_DATA_POINT_H

#include <QMetaType>

#include "storage/Timestamp.h"

class DataPoint {
private:
    Timestamp time_element;
    qreal data_element;
public:
    DataPoint(Timestamp time_element = Timestamp(0), qreal data_element = 0.0);
    
    const Timestamp &get_time_element() const { return time_element; }
    void set_time_element(const Timestamp &new_value) { time_element = new_value; }
    qreal get_data_element() const { return data_element; }
    void set_data_element(qreal new_value) { data_element = new_value; }
    
    DataPoint &operator+=(const DataPoint &other);
};

Q_DECLARE_METATYPE(DataPoint)

#endif
