/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file session/VisualizationRenderer.cpp
*/

#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QPainter>
#include "VisualizationRenderer.h"
#include "VisualizationRenderer.moc"

VisualizationRenderer::VisualizationRenderer(QImage *image, bool can_split)
    : QObject(NULL), EventVisitor(), image(image), can_split(can_split), graph_point(Timestamp(0), 0) {
    
}

VisualizationRenderer::~VisualizationRenderer() {
    
}

QPointF VisualizationRenderer::resolve_point(const VisualizationPoint &point) const {
    const Timestamp &lower_time = range.get_lower_time();
    QRect size; /* FIXME */
    qreal x = qreal(lower_time.ns_until(point.get_time_element()))
        / lower_time.ns_until(range.get_upper_time()) * size.width();
    qreal data_range = range.get_upper_data() - range.get_lower_data();
    qreal y_percentage = (point.get_data_element() - range.get_lower_data()) / data_range;
    qreal y = size.height() * y_percentage;
    y = size.height() - y;
    return QPointF(x, y);
}

QRectF VisualizationRenderer::resolve_range(const VisualizationDataRange &rect) const {
    const Timestamp &lower_time = range.get_lower_time();
    QRect size; /* FIXME */
    qreal x1 = qreal(lower_time.ns_until(rect.get_lower_time()))
        / lower_time.ns_until(range.get_upper_time()) * size.width();
    qreal x2 = qreal(lower_time.ns_until(rect.get_upper_time()))
        / lower_time.ns_until(range.get_upper_time()) * size.width();
    qreal data_range = range.get_upper_data() - range.get_lower_data();
    qreal y1_percentage = (rect.get_lower_data() - range.get_lower_data()) / data_range;
    qreal y2_percentage = (rect.get_upper_data() - range.get_lower_data()) / data_range;
    qreal y1 = size.height() * y1_percentage;
    qreal y2 = size.height() * y2_percentage;
    y1 = size.height() - y1;
    y2 = size.height() - y2;
    return QRectF(x1, y1, x2 - x1, y2 - y1);
}

VisualizationPoint VisualizationRenderer::resolve_point(const QPointF &point) const {
    /* TODO: implement this */
    return VisualizationPoint(Timestamp(0), 0);
}

void VisualizationRenderer::begin_update(Snapshot *starting_snaphot) {
    painter.begin(image);
}

void VisualizationRenderer::end_update() {
    painter.end();
}

void VisualizationRenderer::update_range(const VisualizationDataRange &new_range) {
    range = new_range;
    image->fill(qRgb(128, 128, 128));
}

void VisualizationRenderer::paint_line(const VisualizationPoint &from, const VisualizationPoint &to, QRgb colour, Qt::PenStyle style) {
    QLineF line(resolve_point(from), resolve_point(to));
    
    QPen pen(style);
    pen.setColor(colour);
    painter.setPen(pen);
    painter.drawLine(line);
}

void VisualizationRenderer::paint_box(const VisualizationPoint &from, const VisualizationPoint &to, QRgb line_colour,
    QRgb fill_colour, Qt::PenStyle line_style, Qt::BrushStyle fill_style) {
    
    QPointF from_point = resolve_point(from);
    QPointF to_point = resolve_point(to);
    QRectF rect(from_point, to_point);
    
    QPen pen(line_style);
    pen.setColor(line_colour);
    QBrush brush(fill_style);
    brush.setColor(fill_colour);
    painter.setPen(pen);
    painter.setBrush(brush);
    
    painter.drawRect(rect);
}

void VisualizationRenderer::paint_box(const VisualizationDataRange& range, QRgb line_colour, QRgb fill_colour,
    Qt::PenStyle line_style, Qt::BrushStyle fill_style) {

    QRectF rect = resolve_range(range);
    
    QPen pen(line_style);
    pen.setColor(line_colour);
    QBrush brush(fill_style);
    brush.setColor(fill_colour);
    
    painter.setPen(pen);
    painter.setBrush(brush);
    
    painter.drawRect(rect);
}


void VisualizationRenderer::paint_text(const VisualizationPoint &point, QString text, int size, QRgb colour) {
    QPointF location = resolve_point(point);
    /* TODO: implement this */
    /*QGraphicsTextItem *text_item = new QGraphicsTextItem(text);
    QFont font("DejaVu Sans", 8);
    text_item->setFont(font);
    text_item->translate(location.x(), location.y());
    text_item->setDefaultTextColor(colour);*/
    /*canvas->addItem(text_item);*/
}

void VisualizationRenderer::paint_graph_element(const VisualizationPoint &point, QRgb colour) {
    paint_line(VisualizationPoint(point.get_time_element(), range.get_lower_data()), point, colour);
    if(graph_point_valid == true) {
        paint_line(graph_point, point, colour);
    }
    graph_point = point;
    graph_point_valid = true;
}
