#include "VisualizationRequest.h"

VisualizationRequest::VisualizationRequest(VisualizationCanvas *canvas, const Timestamp &from, const Timestamp &to)
    : from(from), to(to), renderer(NULL), canvas(canvas) {
}
