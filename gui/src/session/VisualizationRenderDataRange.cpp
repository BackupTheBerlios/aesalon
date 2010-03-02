#include "VisualizationRenderDataRange.h"

VisualizationRenderDataRange::VisualizationRenderDataRange()
    : lower_time(Timestamp(0)), upper_time(Timestamp(0)),
    lower_data(0), upper_data(0) {
    
}


VisualizationRenderDataRange::VisualizationRenderDataRange(const Timestamp &lower_time, const Timestamp &upper_time, qint64 lower_data, qint64 upper_data)
    : lower_time(lower_time), upper_time(upper_time), lower_data(lower_data), upper_data(upper_data) {
    
}
