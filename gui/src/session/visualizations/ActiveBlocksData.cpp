#include <QPainter>
#include "ActiveBlocksData.h"

ActiveBlocksData::ActiveBlocksData(const Timestamp &timestamp, int value) : VisualizationData(), timestamp(timestamp), value(value) {
}

ActiveBlocksData::~ActiveBlocksData() {

}

bool ActiveBlocksData::render_onto(QImage* image, VisualizationRequest* request) const {
    if(image == 0) return false;
    const Timestamp &from = request->get_from();
    const Timestamp &to = request->get_to();
    
    qint64 total_time = from.ms_until(to);
    double percentage = from.ms_until(timestamp) / total_time;
    
    qDebug("Trying to render ActiveBlocksData: timestamp is %s, value is %i . . .", timestamp.to_string().toStdString().c_str(), value);
    
    QSize image_size = image->size();
    QPainter painter(image);
    QBrush brush;
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.drawLine(image_size.width() * percentage, 0, image_size.width() * percentage, image_size.height() - (value*20));
    
    qint64 diff = timestamp.ms_until(request->get_to());
    qDebug("Difference is %lli . . .", diff);
    if(qAbs<qint64>(diff) < 500) {
        qDebug("Returning true!");
        return true;
    }
    return false;
}

bool ActiveBlocksData::is_within(VisualizationRequest* request) const {
    if(request->get_from() <= timestamp && request->get_to() >= timestamp) return true;
    return false;
}
