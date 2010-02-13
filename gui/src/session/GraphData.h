#ifndef AESALON_GUI_SESSION_GRAPH_DATA_H
#define AESALON_GUI_SESSION_GRAPH_DATA_H

#include <QtGlobal>

class GraphData {
private:
    qreal value;
public:
    GraphData(qreal value) : value(value) {}
    
    qreal get_value() const { return value; }
};

#endif
