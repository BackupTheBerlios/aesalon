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
    QList<VisualizationRenderDataRange> gaps;
    VisualizationRenderDataRange range;
    QList<VisualizationData *> data_list;
    bool can_split;
    
    VisualizationRenderPoint graph_point;
    bool graph_point_valid;
    
    void recalc_ranges();
    void paint_grid();
    void render_data();
    QPointF resolve_point(const VisualizationRenderPoint &point) const;
public:
    VisualizationRenderer(QPixmap *image, bool can_split);
    virtual ~VisualizationRenderer();
    
    void update(const QSize &canvas_size);
    void add_data(VisualizationData *data);
    
    /* NOTE: this *need* to be reentrant! */
    void paint_line(const VisualizationRenderPoint &from, const VisualizationRenderPoint &to, QRgb colour, Qt::PenStyle style = Qt::SolidLine);
    void paint_text(const VisualizationRenderPoint &point, QString text, int size, QRgb colour);
    void paint_graph_element(const VisualizationRenderPoint &point, QRgb colour);
};

#endif
