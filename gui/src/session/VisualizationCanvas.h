#ifndef AESALON_GUI_SESSION_VISUALIZATION_CANVAS_H
#define AESALON_GUI_SESSION_VISUALIZATION_CANVAS_H

#include <QGraphicsScene>
#include "VisualizationRenderDataRange.h"

class VisualizationCanvas : public QGraphicsScene { Q_OBJECT
private:
    VisualizationRenderDataRange data_range;
public:
    VisualizationCanvas(QObject* parent = 0);
    virtual ~VisualizationCanvas();
    
    const VisualizationRenderDataRange &get_data_range() { return data_range; }
    void set_data_range(const VisualizationRenderDataRange &new_range) { data_range = new_range; }
};

#endif
