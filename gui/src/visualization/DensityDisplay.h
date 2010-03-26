#ifndef AESALON_GUI_VISUALIZATION_DENSITY_DISPLAY_H
#define AESALON_GUI_VISUALIZATION_DENSITY_DISPLAY_H

#include "VisualizationDisplay.h"

class DensityDisplay : public VisualizationDisplay { Q_OBJECT
public:
    DensityDisplay(VisualizationFactory *factory, QWidget* parent);
    virtual ~DensityDisplay();
};

#endif
