#include "DensityFormatter.h"

QString DensityFormatter::format_point(const DataPoint &point) {
    return QString().sprintf("%s, 0x%llx", qPrintable(point.get_time_element().to_string()), quint64(point.get_data_element()));
}
