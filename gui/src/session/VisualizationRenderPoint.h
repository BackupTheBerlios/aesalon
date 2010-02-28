#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDER_POINT_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDER_POINT_H

#include "VisualizationRenderObject.h"

class VisualizationRenderPoint : public VisualizationRenderObject {
public:
    VisualizationRenderPoint(const Timestamp &time, qint64 value);
};

#endif
