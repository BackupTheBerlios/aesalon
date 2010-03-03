#include <QDebug>
#include <QPainter>
#include "VisualizationRenderer.h"

VisualizationRenderer::VisualizationRenderer(QPixmap *image, bool can_split, QWidget *canvas)
    : image(image), can_split(can_split), canvas(canvas) {
    
}

VisualizationRenderer::~VisualizationRenderer() {
    
}


void VisualizationRenderer::recalc_ranges() {
    ranges.clear();
    if(data_list.size() == 0) return;
    if(can_split == false) {
        VisualizationRenderDataRange range = data_list[0]->get_data_range();
        /* TODO: find a workaround for this . . . e.g. support rendering from an offset display . . . */
        range.set_lower_data(0);
        foreach(VisualizationData *data, data_list) {
            if(data->get_data_range().get_lower_time() < range.get_lower_time())
                range.set_lower_time(data->get_data_range().get_lower_time());
            if(data->get_data_range().get_upper_time() > range.get_upper_time())
                range.set_upper_time(data->get_data_range().get_upper_time());
            
            if(data->get_data_range().get_lower_data() < range.get_lower_data())
                range.set_lower_data(data->get_data_range().get_lower_data());
            if(data->get_data_range().get_upper_data() > range.get_upper_data())
                range.set_upper_data(data->get_data_range().get_upper_data());
        }
        range.set_upper_data(range.get_upper_data() * 2);
        ranges.append(range);
    }
    else {
        qCritical("Compressable range calculation NYI!");
        return;
    }
}

void VisualizationRenderer::paint_grid() {
    if(can_split == true) {
        qCritical("Painting split grids NYI!");
        return;
    }
    
    qint64 total_time = ranges[0].get_lower_time().ms_until(ranges[0].get_upper_time());
    qint64 total_data_size = (ranges[0].get_upper_data() - ranges[0].get_lower_data());
    
    for(int x = 0; x < 12; x ++) {
        qint64 value = x * (total_data_size / 12.0);
        paint_line(VisualizationRenderPoint(ranges[0].get_lower_time(), value),
            VisualizationRenderPoint(ranges[0].get_upper_time(), value), qRgb(32, 32, 32), Qt::DotLine);
        QString desc;
        desc.sprintf("%lli", value);
        paint_text(VisualizationRenderPoint(ranges[0].get_lower_time(), value), desc, 8, qRgb(0, 0, 0));
    }
    
    for(int y = 0; y < 12; y ++) {
        Timestamp ts = ranges[0].get_lower_time();
        ts.add_ms(y * (total_time / 12.0));
        paint_line(VisualizationRenderPoint(ts, 0), VisualizationRenderPoint(ts, ranges[0].get_upper_data()), qRgb(32, 32, 32), Qt::DotLine);
        QString desc;
        qint64 time_diff = ranges[0].get_lower_time().ms_until(ts);
        desc.sprintf("%02lli:%02lli.%03lli", (time_diff / 1000) / 60, (time_diff / 1000) % 60, time_diff % 1000);
        paint_text(VisualizationRenderPoint(ts, 0), desc, 8, qRgb(0, 0, 0));
        /*paint_text(VisualizationRenderPoint(ts, ranges[0].get_upper_data()-1), desc, 8, qRgb(0, 0, 0));*/
    }
}

void VisualizationRenderer::render_data() {
    foreach(VisualizationData *data, data_list) {
        data->paint_onto(this);
    }
}

void VisualizationRenderer::update() {
    if(data_list.size() == 0) return;
    if(canvas->geometry() != image->rect()) {
        (*image) = image->scaled(canvas->width(), canvas->height());
    }
    recalc_ranges();
    /* Paint the image white . . . */
    image->fill(qRgb(255, 255, 255));
    paint_grid();
    render_data();
}

void VisualizationRenderer::add_data(VisualizationData *data) {
    data_list.append(data);
}

void VisualizationRenderer::paint_line(VisualizationRenderPoint from, VisualizationRenderPoint to, QRgb colour, Qt::PenStyle style) {
    QLineF line;
    qreal x1, y1, x2, y2;
    if(can_split == false) {
        x1 = qreal(ranges[0].get_lower_time().ms_until(from.get_time_element())) / ranges[0].get_lower_time().ms_until(ranges[0].get_upper_time()) * image->width();
        /*y1 = (qreal(from.get_data_element() - ranges[0].get_lower_data()) / (ranges[0].get_upper_data() - ranges[0].get_lower_data())) * image->height();*/
        x2 = qreal(ranges[0].get_lower_time().ms_until(to.get_time_element())) / ranges[0].get_lower_time().ms_until(ranges[0].get_upper_time()) * image->width();
        /*y2 = (qreal(to.get_data_element() - ranges[0].get_lower_data()) / (ranges[0].get_upper_data() - ranges[0].get_lower_data())) * image->height();*/
        
        qint64 upper_data = ranges[0].get_upper_data();
        qint64 lower_data = ranges[0].get_lower_data();
        /* If height is 100, and the difference is 20, then the scale should be 5.0. */
        /* If height is 100, and the diference is 200, then the scale should be 0.5 . . .  */
        qreal scale = image->height() / qreal(upper_data - lower_data);
        y1 = from.get_data_element() * scale;
        y2 = to.get_data_element() * scale;
        /*qDebug("upper_data: %lli, lower_data: %lli, scale: %f, y1: %f, y2: %f", upper_data, lower_data, scale, y1, y2);*/
    }
    else {
        qCritical("Cannot paint lines in split visualizations (NYI)!");
    }
    line.setLine(x1, image->height() - y1, x2, image->height() - y2);
    QPainter painter(image);
    QPen pen(style);
    pen.setColor(colour);
    painter.setPen(pen);
    painter.drawLine(line);
}

void VisualizationRenderer::paint_text(VisualizationRenderPoint point, QString text, int size, QRgb colour) {
    qreal x = qreal(ranges[0].get_lower_time().ms_until(point.get_time_element())) / ranges[0].get_lower_time().ms_until(ranges[0].get_upper_time()) * image->width();
    qreal y = point.get_data_element() * (image->height() / qreal(ranges[0].get_upper_data() - ranges[0].get_lower_data()));
    QPainter painter(image);
    QPen pen(Qt::SolidLine);
    pen.setColor(colour);
    painter.setPen(pen);
    painter.setFont(QFont("DejaVu Sans", size));
    QPointF pointf(x, image->height() - y);
    painter.drawText(pointf, text);
}
