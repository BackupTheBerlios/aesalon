#ifndef AESALON_GUI_SESSION_VISUALIZATION_AXIS_FORMATTER_H
#define AESALON_GUI_SESSION_VISUALIZATION_AXIS_FORMATTER_H

#include <QString>

#include "VisualizationRenderPoint.h"

class VisualizationAxisFormatter {
public:
    virtual QString format_data(const VisualizationRenderPoint &point) const;
    virtual QString format_time(const VisualizationRenderPoint &point) const;
};

#endif
