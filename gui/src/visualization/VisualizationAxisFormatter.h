#ifndef AESALON_GUI_SESSION_VISUALIZATION_AXIS_FORMATTER_H
#define AESALON_GUI_SESSION_VISUALIZATION_AXIS_FORMATTER_H

#include <QString>

#include "VisualizationPoint.h"

class VisualizationAxisFormatter {
public:
    virtual QString format_data(const VisualizationPoint &point) const;
    virtual QString format_time(const VisualizationPoint &point) const;
};

#endif
