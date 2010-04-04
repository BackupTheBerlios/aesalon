#include <QPainter>
#include "RenderedCanvas.h"
#include "CoordinateMapper.h"

RenderedCanvas::RenderedCanvas(QSize canvas_size, const DataRange &range) : range(range) {
    image = QImage(canvas_size, QImage::Format_RGB32);
    clear();
}

RenderedCanvas::~RenderedCanvas() {

}

void RenderedCanvas::clear() {
    image.fill(qRgb(255, 255, 255));
}

void RenderedCanvas::resize(QSize new_size) {
    image = image.scaled(new_size);
}

void RenderedCanvas::shift(QPointF amount) {
    QPainter painter(&image);
    QImage temporary = image;
    clear();
    painter.drawImage(amount, temporary);
    painter.end();
}

void RenderedCanvas::shift(DataPoint amount) {
    CoordinateMapper mapper(image.size(), range);
    shift(mapper.map_to(amount));
    
    range.get_begin().set_time_element(range.get_begin().get_time_element() + amount.get_time_element());
    range.get_end().set_time_element(range.get_end().get_time_element() + amount.get_time_element());
    
    range.get_begin().set_data_element(range.get_begin().get_data_element() + amount.get_data_element());
    range.get_end().set_data_element(range.get_end().get_data_element() + amount.get_data_element());
}

void RenderedCanvas::merge(RenderedCanvas &canvas) {
    qDebug("Merging RenderedCanvas objects . . .");
    /* If the two canvases do not intersect, don't bother doing anything. */
    if(canvas.range.intersects(range) == false) return;
    CoordinateMapper local_mapper(image.size(), range);
    /*CoordinateMapper canvas_mapper(canvas.image.size(), canvas.range);*/
    QRectF local_rect = local_mapper.map_to(canvas.get_range());
    QRectF canvas_rect = QRectF(QPointF(0, 0), canvas.get_size());
    
    QImage canvas_scaled = canvas.get_image().scaled(QSize(local_rect.width(), local_rect.height()));
    QPainter painter(&image);
    painter.setBrush(QBrush(qRgb(255, 0, 0)));
    painter.drawRect(local_rect);
    painter.drawImage(local_rect, canvas_scaled);
    painter.end();
}
