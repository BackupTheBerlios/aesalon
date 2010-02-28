#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDER_OBJECT_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDER_OBJECT_H

#include "VisualizationRenderDataRange.h"

class VisualizationRenderObject {
private:
    VisualizationRenderDataRange data_range;
public:
    VisualizationRenderObject(const VisualizationRenderDataRange &data_range) : data_range(data_range) {}
    virtual ~VisualizationRenderObject() {}
    
    const VisualizationRenderDataRange &get_data_range() const { return data_range; }
    void set_data_range(const VisualizationRenderDataRange &new_range) { data_range = new_range; }
};

#endif
