#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QPainter>
#include "VisualizationRenderer.h"

VisualizationRenderer::VisualizationRenderer(VisualizationCanvas *canvas, bool can_split)
    : canvas(canvas), can_split(can_split), graph_point(Timestamp(0), 0) {
    
}

VisualizationRenderer::~VisualizationRenderer() {
    
}

QPointF VisualizationRenderer::resolve_point(const VisualizationRenderPoint &point) const {
    const Timestamp &lower_time = range.get_lower_time();
    QRect size = canvas->views()[0]->geometry();
    qreal x = qreal(lower_time.ns_until(point.get_time_element()))
        / lower_time.ns_until(range.get_upper_time()) * size.width();
    qreal data_range = range.get_upper_data() - range.get_lower_data();
    qreal y_percentage = (point.get_data_element() - range.get_lower_data()) / data_range;
    qreal y = size.height() * y_percentage;
    y = size.height() - y;
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
    
    qint64 data_range = (range.get_upper_data()-range.get_lower_data()) / 12;
    if((range.get_upper_data() - range.get_lower_data()) % 12) data_range ++;
    range.set_upper_data((data_range * 12) + range.get_lower_data());
    /*qint64 time_range = range.get_lower_time().ms_until(range.get_upper_time()) / 12;
    Timestamp upper_time = range.get_lower_time();
    upper_time.add_ms(time_range * 12);
    range.set_upper_time(upper_time);*/
    
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
        paint_line(VisualizationRenderPoint(ts, range.get_lower_data()), VisualizationRenderPoint(ts, range.get_upper_data()), qRgb(32, 32, 32), Qt::DotLine);
        QString desc;
        qint64 time_diff = range.get_lower_time().ms_until(ts);
        desc.sprintf("%02lli:%02lli.%03lli", (time_diff / 1000) / 60, (time_diff / 1000) % 60, time_diff % 1000);
        paint_text(VisualizationRenderPoint(ts, range.get_lower_data()), desc, 8, qRgb(0, 0, 0));
    }
}

void VisualizationRenderer::render_data() {
    foreach(VisualizationData *data, data_list) {
        data->paint_onto(this);
    }
}

void VisualizationRenderer::update() {
    graph_point_valid = false;
    if(data_list.size() == 0) return;
    canvas->clear();
    recalc_ranges();
    canvas->set_data_range(range);
    paint_grid();
    render_data();
}

void VisualizationRenderer::add_data(VisualizationData *data) {
    data_list.append(data);
}

void VisualizationRenderer::paint_line(const VisualizationRenderPoint &from, const VisualizationRenderPoint &to, QRgb colour, Qt::PenStyle style) {
    QLineF line(resolve_point(from), resolve_point(to));
    
    QPen pen(style);
    pen.setColor(colour);
    canvas->addLine(line, pen);
}

void VisualizationRenderer::paint_box(const VisualizationRenderPoint &from, const VisualizationRenderPoint &to, QRgb line_colour,
    QRgb fill_colour, Qt::PenStyle line_style, Qt::BrushStyle fill_style) {
    
    QPointF from_point = resolve_point(from);
    QPointF to_point = resolve_point(to);
    QRectF rect(from_point, to_point);
    
    QPen pen(line_style);
    pen.setColor(line_colour);
    QBrush brush(fill_style);
    brush.setColor(fill_colour);
    canvas->addRect(rect, pen, brush);
}

void VisualizationRenderer::paint_text(const VisualizationRenderPoint &point, QString text, int size, QRgb colour) {
    QPointF location = resolve_point(point);
    QGraphicsTextItem *text_item = new QGraphicsTextItem(text);
    QFont font("DejaVu Sans", 8);
    text_item->setFont(font);
    text_item->translate(location.x(), location.y());
    text_item->setDefaultTextColor(colour);
    canvas->addItem(text_item);
}

void VisualizationRenderer::paint_graph_element(const VisualizationRenderPoint &point, QRgb colour) {
    paint_line(VisualizationRenderPoint(point.get_time_element(), range.get_lower_data()), point, colour);
    if(graph_point_valid == true) {
        paint_line(graph_point, point, colour);
    }
    graph_point = point;
    graph_point_valid = true;
}
