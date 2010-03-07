#ifndef AESALON_GUI_SESSION_VISUALIZATION_REQUEST_H
#define AESALON_GUI_SESSION_VISUALIZATION_REQUEST_H

#include "storage/Timestamp.h"
#include "DataRequest.h"
#include "VisualizationRenderer.h"

class VisualizationRequest {
private:
    Timestamp from, to;
    VisualizationRenderer *renderer;
    VisualizationCanvas *canvas;
public:
    VisualizationRequest(VisualizationCanvas *canvas, const Timestamp &from, const Timestamp &to);
    
    const Timestamp &get_from() const { return from; }
    const Timestamp &get_to() const { return to; }
    void set_renderer(VisualizationRenderer *new_renderer) { renderer = new_renderer; }
    VisualizationRenderer *get_renderer() const { return renderer; }
    VisualizationCanvas *get_canvas() const { return canvas; }
};

#endif
