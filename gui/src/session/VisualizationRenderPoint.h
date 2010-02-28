#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDER_POINT_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDER_POINT_H

#include <QtGlobal>

#include "storage/Timestamp.h"

class VisualizationRenderPoint {
private:
    Timestamp time_element;
    qint64 data_element;
public:
    VisualizationRenderPoint(const Timestamp &time, qint64 data) : time_element(time), data_element(data) {}
    
    const Timestamp &get_time() const { return time_element; }
    qint64 get_data() const { return data_element; }
};

#endif
