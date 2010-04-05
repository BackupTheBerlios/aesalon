#include "DataRange.h"

DataRange::DataRange(const DataPoint &begin, const DataPoint &end) : begin(begin), end(end) {

}

DataRange::DataRange(const Timestamp &begin_time, qreal begin_data, const Timestamp &end_time, qreal end_data)
    : begin(begin_time, begin_data), end(end_time, end_data) {
}

bool DataRange::intersects(const DataRange &range) const {
    
    /*
        Non-intersection:
        A---\
        |   |
        \---B
        
                C---\
                |   |
                \---D
        
        Intersection:
        A---\
        |  C+-\
        \--+B |
           \--D
        
        if C >= B, or if A >= D, then the rectangles do not intersect.
        
        Given A and B:
            A == B when:
                a.x == b.x and a.y == b.y
            A > B when:
                a.x > b.x and a.y > b.y
            A >= B when:
                A == B or A > B
    */
    
    /* compare end to range.begin(), and begin to range.end() . . .*/
    if(range.begin.get_time_element() >= end.get_time_element() || range.begin.get_data_element() >= end.get_data_element()) return false;
    else if(begin.get_time_element() >= range.end.get_time_element() || begin.get_data_element() >= range.end.get_data_element()) return false;
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

bool DataRange::contains(const DataPoint &point) const {
    if(point.get_data_element() < begin.get_data_element() || point.get_time_element() < begin.get_time_element()
        || point.get_data_element() > end.get_data_element() || point.get_time_element() > end.get_time_element()) return false;
    return true;
}
