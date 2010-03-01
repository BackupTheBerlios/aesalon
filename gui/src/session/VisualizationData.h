#ifndef AESALON_GUI_SESSION_VISUALIZATION_DATA_H
#define AESALON_GUI_SESSION_VISUALIZATION_DATA_H

#include <QImage>

#include "VisualizationRenderDataRange.h"

class VisualizationRequest;
class VisualizationRenderer;

class VisualizationData {
private:
    VisualizationRenderDataRange data_range;
public:
    VisualizationData();
    virtual ~VisualizationData();
    
    virtual void create_r_objects(VisualizationRenderer *renderer) const = 0;
    
    virtual bool is_within(VisualizationRequest *request) const = 0;
    virtual bool is_cachable() const = 0;
    
    VisualizationRenderDataRange &get_data_range() { return data_range; }
};

#endif
