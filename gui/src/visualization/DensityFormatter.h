#ifndef AESALON_GUI_VISUALIZATION_DENSITY_FORMATTER_H
#define AESALON_GUI_VISUALIZATION_DENSITY_FORMATTER_H

#include "AxisFormatter.h"

class DensityFormatter : public AxisFormatter {
public:
    virtual QString format_point(const DataPoint& point);
};

#endif
