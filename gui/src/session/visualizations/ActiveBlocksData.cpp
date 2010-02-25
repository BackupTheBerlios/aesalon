#include <QPainter>
#include "ActiveBlocksData.h"

ActiveBlocksData::ActiveBlocksData(const Timestamp &timestamp, int value) : VisualizationData(), timestamp(timestamp), value(value) {
}

ActiveBlocksData::~ActiveBlocksData() {

}

void ActiveBlocksData::render_onto(QImage* image, VisualizationRequest* request) const {
    if(image == 0) return;
    const Timestamp &from = request->get_from();
    const Timestamp &to = request->get_to();
    
    qint64 total_time = from.ms_until(to);
    double percentage = from.ms_until(timestamp) / double(total_time);
    
    QSize image_size = image->size();
    QPainter painter(image);
    QBrush brush;
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.drawLine(static_cast<int>(image_size.width() * percentage), image_size.height(), static_cast<int>(image_size.width() * percentage), image_size.height()-(value));
}

bool ActiveBlocksData::is_within(VisualizationRequest* request) const {
    if(request->get_from() <= timestamp && request->get_to() >= timestamp) return true;
    return false;
}
