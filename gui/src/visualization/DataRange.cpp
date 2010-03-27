#include "DataRange.h"

DataRange::DataRange(const DataPoint &begin, const DataPoint &end) : begin(begin), end(end) {

}

DataRange::DataRange(const Timestamp &begin_time, qreal begin_data, const Timestamp &end_time, qreal end_data)
    : begin(begin_time, begin_data), end(end_time, end_data) {
}

bool DataRange::intersects(const DataRange &range) const {
    if(range.end.get_data_element() < begin.get_data_element() || range.end.get_time_element() < begin.get_time_element()
        || range.begin.get_data_element() > end.get_data_element() || range.begin.get_time_element() > end.get_time_element()) return false;
    return true;
}

DataRange DataRange::intersecting_range(const DataRange &other_range) const {
    return DataRange(
        qMax(other_range.get_begin().get_time_element(), begin.get_time_element()),
        qMax(other_range.get_begin().get_data_element(), begin.get_data_element()),
        qMin(other_range.get_end().get_time_element(), end.get_time_element()),
        qMin(other_range.get_end().get_data_element(), end.get_data_element()));
}

DataRange DataRange::encompassing_range(const DataRange &other_range) const {
    return DataRange(
        qMin(other_range.get_begin().get_time_element(), begin.get_time_element()),
        qMin(other_range.get_begin().get_data_element(), begin.get_data_element()),
        qMax(other_range.get_end().get_time_element(), end.get_time_element()),
        qMax(other_range.get_end().get_data_element(), end.get_data_element()));
}
