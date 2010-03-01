#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDER_POINT_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDER_POINT_H

#include "storage/Timestamp.h"

class VisualizationRenderPoint {
private:
    Timestamp time_element;
    qint64 data_element;
public:
    VisualizationRenderPoint(const Timestamp &time, qint64 data) : time_element(time), data_element(data) {}
    
    const Timestamp &get_time_element() const { return time_element; }
    void set_time_element(const Timestamp &new_time) { time_element = new_time; }
    
    qint64 get_data_element() const { return data_element; }
    void set_data_element(qint64 new_data) { data_element = new_data; }
};

#endif
