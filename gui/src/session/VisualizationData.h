#ifndef AESALON_GUI_SESSION_VISUALIZATION_DATA_H
#define AESALON_GUI_SESSION_VISUALIZATION_DATA_H

#include <QImage>

#include "VisualizationRequest.h"
#include "VisualizationRenderDataRange.h"

class VisualizationData {
private:
    VisualizationRenderDataRange data_range;
public:
    VisualizationData();
    virtual ~VisualizationData();
    
    virtual void render_onto(QImage *image, VisualizationRequest *request) const = 0;
    
    virtual bool is_within(VisualizationRequest *request) const = 0;
    virtual bool is_cachable() const = 0;
    
    const VisualizationRenderDataRange &get_data_range() const { return data_range; }
protected:
    VisualizationRenderDataRange &get_data_range() { return data_range; }
};

#endif
