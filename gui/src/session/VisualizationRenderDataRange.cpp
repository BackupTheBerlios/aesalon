#include "VisualizationRenderDataRange.h"

VisualizationRenderDataRange::VisualizationRenderDataRange() {
    
}


VisualizationRenderDataRange::VisualizationRenderDataRange(const Timestamp &lower_time, const Timestamp &upper_time, qint64 lower_data, qint64 upper_data)
    : lower_time(lower_time), upper_time(upper_time), lower_data(lower_data), upper_data(upper_data) {
    
}
