#ifndef AESALON_GUI_VISUALIZATION_DENSITY_FACTORY_H
#define AESALON_GUI_VISUALIZATION_DENSITY_FACTORY_H

#include "VisualizationFactory.h"

class DensityFactory : public VisualizationFactory {
public:
    DensityFactory();
    virtual ~DensityFactory();
    
    virtual VisualizationDisplay *create_display(QWidget *parent) const;
    virtual VisualizationRenderer *create_renderer(QImage *image) const;
};

#endif
