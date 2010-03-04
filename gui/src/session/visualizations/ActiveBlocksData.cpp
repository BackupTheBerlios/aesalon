#include <QPainter>
#include "ActiveBlocksData.h"
#include "../VisualizationRenderer.h"
#include "../VisualizationRequest.h"

ActiveBlocksData::ActiveBlocksData(const Timestamp &timestamp, int value) : VisualizationData(), timestamp(timestamp), value(value) {
    get_data_range().set_lower_data(value);
    get_data_range().set_upper_data(value);
    get_data_range().set_lower_time(timestamp);
    get_data_range().set_upper_time(timestamp);
}

ActiveBlocksData::~ActiveBlocksData() {
}

void ActiveBlocksData::paint_onto(VisualizationRenderer *renderer) const {
    renderer->paint_graph_element(VisualizationRenderPoint(timestamp, value), qRgb(0, 0, 0));
}

bool ActiveBlocksData::is_within(VisualizationRequest* request) const {
    if(request->get_from() <= timestamp && request->get_to() >= timestamp) return true;
    return false;
}
