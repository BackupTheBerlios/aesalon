#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDER_DATA_RANGE_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDER_DATA_RANGE_H

#include <QtGlobal>
#include <QRect>

#include "storage/Timestamp.h"

class VisualizationRenderDataRange {
private:
    Timestamp lower_time, upper_time;
    qint64 lower_data, upper_data;
    QRect image_coords;
public:
    VisualizationRenderDataRange();
    VisualizationRenderDataRange(const Timestamp &lower_time, const Timestamp &upper_time, qint64 lower_data, qint64 upper_data);
    
    const Timestamp &get_lower_time() const { return lower_time; }
    void set_lower_time(const Timestamp &new_time) { lower_time = new_time; }
    const Timestamp &get_upper_time() const { return upper_time; }
    void set_upper_time(const Timestamp &new_time) { upper_time = new_time; }
    qint64 get_lower_data() const { return lower_data; }
    void set_lower_data(qint64 new_data) { lower_data = new_data; }
    qint64 get_upper_data() const { return upper_data; }
    void set_upper_data(qint64 new_data) { upper_data = new_data; }
    
    const QRect &get_image_coords() const { return image_coords; }
    void set_image_coords(const QRect &new_coords) { image_coords = new_coords; }
};

#endif
