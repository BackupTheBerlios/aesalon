#ifndef AESALON_GUI_SESSION_VISUALIZATION_DATA_H
#define AESALON_GUI_SESSION_VISUALIZATION_DATA_H

#include "VisualizationRequest.h"

class VisualizationData {
public:
    VisualizationData();
    virtual ~VisualizationData();
    
    virtual bool is_within(VisualizationRequest *request) const = 0;
};

#endif
