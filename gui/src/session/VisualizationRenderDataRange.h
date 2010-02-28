#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDER_DATA_RANGE_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDER_DATA_RANGE_H

#include <QtGlobal>

#include "storage/Timestamp.h"

class VisualizationRenderDataRange {
private:
    Timestamp lower_time, upper_time;
    qint64 lower_data, upper_data;
public:
    VisualizationRenderDataRange();
    
    const Timestamp &get_lower_time() const { return lower_time; }
    const Timestamp &get_upper_time() const { return upper_time; }
    qint64 get_lower_data() const { return lower_data; }
    qint64 get_upper_data() const { return upper_data; }
};

#endif
