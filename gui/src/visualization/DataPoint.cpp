#include "DataPoint.h"

DataPoint::DataPoint(Timestamp time_element, qreal data_element)
    : time_element(time_element), data_element(data_element) {

}

DataPoint &DataPoint::operator+=(const DataPoint &other) {
    time_element += other.time_element;
    data_element += other.data_element;
    return *this;
}
