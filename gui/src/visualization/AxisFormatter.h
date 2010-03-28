#ifndef AESALON_GUI_VISUALIZATION_AXIS_FORMATTER_H
#define AESALON_GUI_VISUALIZATION_AXIS_FORMATTER_H

#include <QString>

#include "DataPoint.h"

class AxisFormatter {
public:
    virtual QString format_point(const DataPoint &point) = 0;
};

#endif
