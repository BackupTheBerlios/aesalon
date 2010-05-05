#ifndef AESALON_GUI_VISUALIZATION_DENSITY_FACTORY_H
#define AESALON_GUI_VISUALIZATION_DENSITY_FACTORY_H

#include "VisualizationFactory.h"

class DensityFactory : public VisualizationFactory {
public:
    DensityFactory(DataThread *data_thread);

    virtual Renderer *create_renderer(Canvas *canvas) const;
    virtual AxisFormatter *create_formatter() const;
    virtual ClickHandler *create_click_handler(QWidget *info_widget) const;
    
    virtual QString get_vertical_axis_label() const { return "Address space"; }
};

#endif
