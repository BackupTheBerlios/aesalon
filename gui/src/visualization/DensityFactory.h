#ifndef AESALON_GUI_VISUALIZATION_DENSITY_FACTORY_H
#define AESALON_GUI_VISUALIZATION_DENSITY_FACTORY_H

#include "VisualizationFactory.h"

class DensityFactory : public VisualizationFactory {
public:
    DensityFactory(DataThread *data_thread);

    virtual Renderer *create_renderer() const;
    virtual AxisFormatter *create_formatter() const;
    virtual ClickHandler *create_click_handler() const;
};

#endif
