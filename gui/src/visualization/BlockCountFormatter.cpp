#include "BlockCountFormatter.h"

QString BlockCountFormatter::format_point(const DataPoint &point) {
    return QString().sprintf("%s, %.0f", qPrintable(point.get_time_element().to_string()), point.get_data_element());
}
