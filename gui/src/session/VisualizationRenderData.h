#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDER_DATA_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDER_DATA_H

#include "VisualizationRenderDataRange.h"

class VisualizationRenderData {
private:
    VisualizationRenderDataRange range;
public:
    VisualizationRenderData();
    virtual ~VisualizationRenderData();
    VisualizationRenderDataRange &get_data_range() { return range; }
    
};

#endif
