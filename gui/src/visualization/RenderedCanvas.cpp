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
        qDebug("amount.x(): %f, amount.y(): %f", amount.x(), amount.y());
        qDebug("Clearing RenderedCanvas, range to shift is greater than image dimensions . . .");
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
    qDebug("RenderedCanvas::shift(DataPoint): amount is (%s, %f)", qPrintable(amount.get_time_element().to_string()), amount.get_data_element());
    qDebug("RenderedCanvas::shift(DataPoint): size is (%i, %i)", image.width(), image.height());
    CoordinateMapper mapper(image.size(), range);
    DataPoint adjusted = amount;
    adjusted += range.get_begin();
    QPointF shift_amount = mapper.map_to(adjusted);
    shift_amount.setY(image.height() - shift_amount.y());
    qDebug("RenderedCanvas::shift(DataPoint): shift vector is (%f, %f) . . .", shift_amount.x(), shift_amount.y());
    shift(shift_amount);
    
    range.get_begin().set_time_element(range.get_begin().get_time_element() + amount.get_time_element());
    range.get_end().set_time_element(range.get_end().get_time_element() + amount.get_time_element());
    
    range.get_begin().set_data_element(range.get_begin().get_data_element() + amount.get_data_element());
    range.get_end().set_data_element(range.get_end().get_data_element() + amount.get_data_element());
}

void RenderedCanvas::merge(RenderedCanvas &canvas) {
    /* If the two canvases do not intersect, don't bother doing anything. */
    if(range.intersects(canvas.range) == false) {
        /*qDebug("Canvas objects do not intersect, returning . . .");
        qDebug("range is \t\t\t(%s, %f),\t(%s, %f)",
            qPrintable(range.get_begin().get_time_element().to_string()), range.get_begin().get_data_element(),
            qPrintable(range.get_end().get_time_element().to_string()), range.get_end().get_data_element());
        qDebug("canvas.range is \t\t(%s,%f),\t(%s,%f)",
            qPrintable(canvas.range.get_begin().get_time_element().to_string()), canvas.range.get_begin().get_data_element(),
            qPrintable(canvas.range.get_end().get_time_element().to_string()), canvas.range.get_end().get_data_element());*/
        return;
    }
    CoordinateMapper local_mapper(image.size(), range);
    /*CoordinateMapper canvas_mapper(canvas.image.size(), canvas.range);*/
    QRectF local_rect = local_mapper.map_to(canvas.get_range());
    QRectF canvas_rect = QRectF(QPointF(0, 0), canvas.get_size());
    
    qreal scaled_width = qAbs(local_rect.width());
    qreal scaled_height = qAbs(local_rect.height());
    QImage canvas_scaled = canvas.get_image().scaled(scaled_width, scaled_height);
    QPainter painter(&image);
    painter.drawImage(local_rect.topLeft(), canvas_scaled);
    painter.end();
}
