#ifndef AESALON_GUI_VISUALIZATION_BLOCK_COUNT_FORMATTER_H
#define AESALON_GUI_VISUALIZATION_BLOCK_COUNT_FORMATTER_H

#include "AxisFormatter.h"

class BlockCountFormatter : public AxisFormatter {
public:
    virtual QString format_point(const DataPoint& point);
};

#endif
