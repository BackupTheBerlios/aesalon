#include <QPainter>
#include "VisualizationRenderer.h"

VisualizationRenderer::VisualizationRenderer(QImage *image, bool can_split)
    : image(image), can_split(can_split) {
    
}

VisualizationRenderer::~VisualizationRenderer() {
    
}


void VisualizationRenderer::recalc_ranges() {
    ranges.clear();
    if(can_split == false) {
        VisualizationRenderDataRange range = VisualizationRenderDataRange();
        foreach(VisualizationData *data, data_list) {
            if(data->get_data_range().get_lower_time() < range.get_lower_time())
                range.set_lower_time(data->get_data_range().get_lower_time());
            if(data->get_data_range().get_upper_time() < range.get_upper_time())
                range.set_upper_time(data->get_data_range().get_upper_time());
            
            if(data->get_data_range().get_lower_data() < range.get_lower_data())
                range.set_lower_data(data->get_data_range().get_lower_data());
            if(data->get_data_range().get_upper_data() < range.get_upper_data())
                range.set_upper_data(data->get_data_range().get_upper_data());
        }
        ranges.append(range);
    }
    else {
        qCritical("Compressable range calculation NYI!");
        return;
    }
}

void VisualizationRenderer::paint_grid() {
    /* Just ignore the grid for now . . . */
}

void VisualizationRenderer::render_data() {
    foreach(VisualizationData *data, data_list) {
        data->paint_onto(this);
    }
}

void VisualizationRenderer::update() {
    recalc_ranges();
    /* Paint the image white . . . */
    image->fill(qRgb(255, 255, 255));
    paint_grid();
    render_data();
}

void VisualizationRenderer::add_data(VisualizationData *data) {
    data_list.append(data);
}

void VisualizationRenderer::paint_line(VisualizationRenderPoint from, VisualizationRenderPoint to, QRgb colour) {
    QLineF line;
    qreal x1, y1, x2, y2;
    if(can_split == false) {
        x1 = qreal(ranges[0].get_lower_time().ms_until(from.get_time_element())) / ranges[0].get_lower_time().ms_until(ranges[0].get_upper_time()) * image->width();
        y1 = (qreal(from.get_data_element() - ranges[0].get_lower_data()) / (ranges[0].get_upper_data() - ranges[0].get_lower_data())) * image->height();
        x2 = qreal(ranges[0].get_lower_time().ms_until(to.get_time_element())) / ranges[0].get_lower_time().ms_until(ranges[0].get_upper_time()) * image->width();
        y2 = (qreal(to.get_data_element() - ranges[0].get_lower_data()) / (ranges[0].get_upper_data() - ranges[0].get_lower_data())) * image->height();
    }
    else {
        qCritical("Cannot paint lines in split visualizations!");
    }
    line.setLine(x1, y1, x2, y2);
    QPainter painter(image);
    qDebug("Drawing line from (%f,%f) to (%f,%f) . . .", x1, y1, x2, y2);
    painter.drawLine(line);
}
