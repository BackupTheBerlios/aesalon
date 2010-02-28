#include "VisualizationRenderPoint.h"

VisualizationRenderPoint::VisualizationRenderPoint(const Timestamp &time, qint64 value)
    : VisualizationRenderObject(VisualizationRenderDataRange(time, time, value, value)) {
}
