#ifndef AESALON_GUI_SESSION_VISUALIZATION_CANVAS_H
#define AESALON_GUI_SESSION_VISUALIZATION_CANVAS_H

#include <QGraphicsScene>
#include "VisualizationRenderDataRange.h"

class VisualizationCanvas : public QGraphicsScene { Q_OBJECT
public:
    VisualizationCanvas(QObject* parent = 0);
    virtual ~VisualizationCanvas();
};

#endif
