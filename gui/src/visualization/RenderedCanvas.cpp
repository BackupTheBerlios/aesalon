#include <QPainter>
#include "RenderedCanvas.h"
#include "CoordinateMapper.h"

RenderedCanvas::RenderedCanvas() {
    clear();
}

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
    /* If the shifting amount is more than the width or height of the image, then just take a shortcut. */
    if(qAbs(amount.x()) > image.width() || qAbs(amount.y()) > image.height()) {
        clear();
        return;
    }
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
    /* If the two canvases do not intersect, don't bother doing anything. */
    if(range.intersects(canvas.range) == false) {
        qDebug("Canvas objects do not intersect, returning . . .");
        return;
    }
    CoordinateMapper local_mapper(image.size(), range);
    /*CoordinateMapper canvas_mapper(canvas.image.size(), canvas.range);*/
    QRectF local_rect = local_mapper.map_to(canvas.get_range());
    qDebug("local_rect: (%f,%f),(%f,%f)", local_rect.left(), local_rect.top(), local_rect.right(), local_rect.bottom());
    QRectF canvas_rect = QRectF(QPointF(0, 0), canvas.get_size());
    qDebug("canvas_rect: (%f,%f),(%f,%f)", canvas_rect.left(), canvas_rect.top(), canvas_rect.right(), canvas_rect.bottom());
    
    qreal scaled_width = qAbs(local_rect.width());
    qreal scaled_height = qAbs(local_rect.height());
    QImage canvas_scaled = canvas.get_image().scaled(scaled_width, scaled_height);
    qDebug("scaled_width: %f, scaled_height: %f", scaled_width, scaled_height);
    qDebug("canvas: width is %i, height is %i . . .", canvas.get_image().width(), canvas.get_image().height());
    qDebug("canvas_scaled: width is %i, height is %i . . .", canvas_scaled.width(), canvas_scaled.height());
    QPainter painter(&image);
    /*image.fill(qRgb(255, 255, 255));*/
    painter.setBrush(QBrush(qRgb(qrand() % 256, qrand() % 256, qrand() % 256)));
    painter.drawRect(local_rect);
    painter.drawImage(local_rect.topLeft(), canvas_scaled);
    painter.end();
}
