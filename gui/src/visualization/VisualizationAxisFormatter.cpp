#include "VisualizationAxisFormatter.h"

QString VisualizationAxisFormatter::format_data(const VisualizationPoint &point) const {
    return QString().setNum(point.get_data_element());
}

QString VisualizationAxisFormatter::format_time(const VisualizationPoint &point) const {
    return point.get_time_element().to_string();
}
