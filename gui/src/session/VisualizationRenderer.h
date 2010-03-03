#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDERER_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDERER_H

#include <QPixmap>
#include <QWidget>

#include "VisualizationRenderDataRange.h"
#include "VisualizationRenderPoint.h"
#include "VisualizationData.h"

class VisualizationRenderer {
private:
    QPixmap *image;
    QList<VisualizationRenderDataRange> ranges;
    QList<VisualizationData *> data_list;
    bool can_split;
    QWidget *canvas;
    
    void recalc_ranges();
    void paint_grid();
    void render_data();
public:
    VisualizationRenderer(QPixmap *image, bool can_split, QWidget *canvas);
    virtual ~VisualizationRenderer();
    
    void update();
    void add_data(VisualizationData *data);
    
    /* NOTE: this *need* to be reentrant! */
    void paint_line(VisualizationRenderPoint from, VisualizationRenderPoint to, QRgb colour);
};

#endif
