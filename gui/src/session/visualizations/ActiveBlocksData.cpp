#include <QPainter>
#include "ActiveBlocksData.h"
#include "../VisualizationRenderer.h"
#include "../VisualizationRequest.h"

ActiveBlocksData::ActiveBlocksData(const Timestamp &timestamp, int value) : VisualizationData(), timestamp(timestamp), value(value) {
}

ActiveBlocksData::~ActiveBlocksData() {

}

void ActiveBlocksData::create_r_objects(VisualizationRenderer *renderer) const {
    
}


bool ActiveBlocksData::is_within(VisualizationRequest* request) const {
    if(request->get_from() <= timestamp && request->get_to() >= timestamp) return true;
    return false;
}
