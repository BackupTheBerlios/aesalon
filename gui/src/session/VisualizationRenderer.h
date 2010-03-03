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
    
    void recalc_ranges();
    void paint_grid();
    void render_data();
public:
    VisualizationRenderer(QPixmap *image, bool can_split);
    virtual ~VisualizationRenderer();
    
    void update(const QSize &canvas_size);
    void add_data(VisualizationData *data);
    
    /* NOTE: this *need* to be reentrant! */
    void paint_line(VisualizationRenderPoint from, VisualizationRenderPoint to, QRgb colour, Qt::PenStyle style = Qt::SolidLine);
    void paint_text(VisualizationRenderPoint point, QString text, int size, QRgb colour);
};

#endif
