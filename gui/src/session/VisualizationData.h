#ifndef AESALON_GUI_SESSION_VISUALIZATION_DATA_H
#define AESALON_GUI_SESSION_VISUALIZATION_DATA_H

#include <QImage>

#include "VisualizationRequest.h"

class VisualizationData {
public:
    VisualizationData();
    virtual ~VisualizationData();
    
    virtual void render_onto(QImage *image, VisualizationRequest *request) const = 0;
    
    virtual bool is_within(VisualizationRequest *request) const = 0;
    virtual bool is_cachable() const = 0;
};

#endif
