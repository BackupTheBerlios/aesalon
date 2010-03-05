#include <QDebug>
#include <QPainter>
#include "VisualizationRenderer.h"

VisualizationRenderer::VisualizationRenderer(QPixmap *image, bool can_split)
    : image(image), can_split(can_split), graph_point(Timestamp(0), 0) {
    
}

VisualizationRenderer::~VisualizationRenderer() {
    
}

QPointF VisualizationRenderer::resolve_point(const VisualizationRenderPoint &point) const {
    const Timestamp &lower_time = range.get_lower_time();
    qreal x = qreal(lower_time.ms_until(point.get_time_element()))
        / lower_time.ms_until(range.get_upper_time()) * image->width();
    qreal data_range = range.get_upper_data() - range.get_lower_data();
    qreal y_percentage = (point.get_data_element() - range.get_lower_data()) / data_range;
    qreal y = image->height() * y_percentage;
    y = image->height() - y;
    return QPointF(x, y);
}

void VisualizationRenderer::recalc_ranges() {
    if(data_list.size() == 0) return;
    
    range = data_list[0]->get_data_range();
    /* TODO: find a workaround for this . . . e.g. support rendering from an offset display . . . */
    /*range.set_lower_data(0);*/
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
    
    qint64 upper_range = range.get_upper_data() / 12;
    if(range.get_upper_data() % 12) upper_range ++;
    
    range.set_upper_data(upper_range * 12);
    
    if(can_split == false) {
        
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
    
    qint64 total_time = range.get_lower_time().ms_until(range.get_upper_time());
    qint64 total_data_size = (range.get_upper_data() - range.get_lower_data());
    
    for(int x = 0; x < 12; x ++) {
        qint64 value = (x * (total_data_size / 12.0)) + range.get_lower_data();
        paint_line(VisualizationRenderPoint(range.get_lower_time(), value),
            VisualizationRenderPoint(range.get_upper_time(), value), qRgb(32, 32, 32), Qt::DotLine);
        QString desc;
        desc.sprintf("%lli", value);
        paint_text(VisualizationRenderPoint(range.get_lower_time(), value), desc, 8, qRgb(0, 0, 0));
    }
    
    for(int y = 0; y < 12; y ++) {
        Timestamp ts = range.get_lower_time();
        ts.add_ms(y * (total_time / 12.0));
        paint_line(VisualizationRenderPoint(ts, 0), VisualizationRenderPoint(ts, range.get_upper_data()), qRgb(32, 32, 32), Qt::DotLine);
        QString desc;
        qint64 time_diff = range.get_lower_time().ms_until(ts);
        desc.sprintf("%02lli:%02lli.%03lli", (time_diff / 1000) / 60, (time_diff / 1000) % 60, time_diff % 1000);
        paint_text(VisualizationRenderPoint(ts, 0), desc, 8, qRgb(0, 0, 0));
    }
}

void VisualizationRenderer::render_data() {
    foreach(VisualizationData *data, data_list) {
        data->paint_onto(this);
    }
}

void VisualizationRenderer::update(const QSize &canvas_size) {
    graph_point_valid = false;
    if(data_list.size() == 0) return;
    if(canvas_size != image->size()) {
        (*image) = image->scaled(canvas_size);
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

void VisualizationRenderer::paint_line(const VisualizationRenderPoint &from, const VisualizationRenderPoint &to, QRgb colour, Qt::PenStyle style) {
    /*qDebug("Painting line from (%s,%lli) to (%s,%lli) . . .", qPrintable(from.get_time_element().to_string()), from.get_data_element(),
        qPrintable(to.get_time_element().to_string()), to.get_data_element());*/
    QPointF from_point = resolve_point(from);
    QPointF to_point = resolve_point(to);
    QLineF line(from_point, to_point);
    
    QPainter painter(image);
    QPen pen(style);
    
    pen.setColor(colour);
    painter.setPen(pen);
    painter.drawLine(line);
}

void VisualizationRenderer::paint_box(const VisualizationRenderPoint &from, const VisualizationRenderPoint &to, QRgb line_colour,
    QRgb fill_colour, Qt::PenStyle line_style, Qt::BrushStyle fill_style) {
    
    QPointF from_point = resolve_point(from);
    QPointF to_point = resolve_point(to);
    QRectF rect(from_point, to_point);
    
    QPainter painter(image);
    QPen pen(line_style);
    pen.setColor(line_colour);
    painter.setPen(pen);
    QBrush brush(fill_style);
    brush.setColor(fill_colour);
    painter.setBrush(brush);
    painter.drawRect(rect);
}

void VisualizationRenderer::paint_text(const VisualizationRenderPoint &point, QString text, int size, QRgb colour) {
    QPointF location = resolve_point(point);
    QPainter painter(image);
    QPen pen(Qt::SolidLine);
    pen.setColor(colour);
    painter.setPen(pen);
    painter.setFont(QFont("DejaVu Sans", size));
    painter.drawText(location, text);
}

void VisualizationRenderer::paint_graph_element(const VisualizationRenderPoint &point, QRgb colour) {
    paint_line(VisualizationRenderPoint(point.get_time_element(), 0), point, colour);
    if(graph_point_valid == true) {
        paint_line(graph_point, point, colour);
    }
    graph_point = point;
    graph_point_valid = true;
}
