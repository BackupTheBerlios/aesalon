#ifndef AESALON_GUI_VISUALIZATION_DATA_RANGE_H
#define AESALON_GUI_VISUALIZATION_DATA_RANGE_H

#include "DataPoint.h"

class DataRange {
private:
    DataPoint begin, end;
public:
    DataRange(const DataPoint &begin, const DataPoint &end);
    DataRange(const Timestamp &begin_time = Timestamp(0), qreal begin_data = 0.0,
        const Timestamp &end_time = Timestamp(0), qreal end_data = 0.0);
    
    const DataPoint &get_begin() const { return begin; }
    DataPoint &get_begin() { return begin; }
    const DataPoint &get_end() const { return end; }
    DataPoint &get_end() { return end; }
    
    bool intersects(const DataRange &range) const;
    DataRange intersecting_range(const DataRange &other_range) const;
    DataRange encompassing_range(const DataRange &other_range) const;
    
    bool contains(const DataPoint &point) const;
    
    DataRange normalized() const;
};

Q_DECLARE_METATYPE(DataRange)

#endif
