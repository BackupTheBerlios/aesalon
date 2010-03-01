#include "VisualizationRequest.h"

VisualizationRequest::VisualizationRequest(const Timestamp &from, const Timestamp &to)
    : from(from), to(to), renderer(NULL) {
}
