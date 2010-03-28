#ifndef AESALON_GUI_VISUALIZATION_DENSITY_FACTORY_H
#define AESALON_GUI_VISUALIZATION_DENSITY_FACTORY_H

#include "VisualizationFactory.h"

class DensityFactory : public VisualizationFactory {
public:
    virtual Renderer *create_renderer() const;
    virtual AxisFormatter *create_formatter() const;
};

#endif
