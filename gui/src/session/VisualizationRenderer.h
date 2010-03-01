#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDERER_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDERER_H

#include <QImage>

#include "VisualizationRenderDataRange.h"
#include "VisualizationData.h"
#include "VisualizationRenderObject.h"

class VisualizationRenderer {
private:
    QImage *image;
    QList<VisualizationRenderDataRange> ranges;
    QList<VisualizationRenderObject *> render_object_list;
    bool can_split;
    
    void recalc_ranges();
    void paint_grid();
    void render_objects();
public:
    VisualizationRenderer(QImage *image, bool can_split);
    virtual ~VisualizationRenderer();
    
    void update();
    
    void add_data(VisualizationData *data);
    void add_render_object(VisualizationRenderObject *r_object);
};

#endif
