#ifndef AESALON_GUI_SESSION_GRAPH_DATA_H
#define AESALON_GUI_SESSION_GRAPH_DATA_H

#include <QtGlobal>
#include "storage/Timestamp.h"

class GraphData {
private:
    qreal value;
    Timestamp timestamp;
public:
    GraphData(const Timestamp &timestamp, qreal value) : value(value), timestamp(timestamp) {}
    
    qreal get_value() const { return value; }
    const Timestamp &get_timestamp() const { return timestamp; }
};

#endif
