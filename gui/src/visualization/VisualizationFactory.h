#ifndef AESALON_GUI_VISUALIZATION_FACTORY_H
#define AESALON_GUI_VISUALIZATION_FACTORY_H

#include "Renderer.h"
#include "AxisFormatter.h"

class VisualizationFactory {
public:
    virtual Renderer *create_renderer() const = 0;
    virtual AxisFormatter *create_formatter() const = 0;
};

#endif
