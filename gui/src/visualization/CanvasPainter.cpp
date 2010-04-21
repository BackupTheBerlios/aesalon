#include <QDebug>

#include "CanvasPainter.h"
#include "CanvasPainter.moc"

CanvasPainter::CanvasPainter(DataThread *data_thread) : QThread(NULL), data_thread(data_thread) {
    
}

CanvasPainter::~CanvasPainter() {
    
}

void CanvasPainter::paint_canvas(QSize render_size, Canvas *canvas) {
    processing = true;
    
    DataRange range = canvas->calculate_data_range();
    RenderedCanvas rendered(render_size, range);
    QPainter painter(&rendered.get_image());
    CoordinateMapper mapper(render_size, range);
    
    
    QRect rect = mapper.map_to(DataRange(Timestamp(0), 0, data_thread->get_last_time(), 0)).toRect();
    rect.setTop(0);
    rect.setBottom(render_size.height() - 1);
    painter.setPen(Qt::NoPen);
    QBrush brush;
    brush.setColor(qRgb(240, 240, 240));
    painter.setBrush(brush);
    painter.drawRect(rect);
    
    CanvasObject *object;
    object = canvas->get_head();
    while(object) {
        object->paint_onto(&painter, mapper);
        object = object->get_next();
    }
    processing = false;
    emit done(rendered);
}

void CanvasPainter::paint_canvas(QSize render_size, Canvas *canvas, DataRange range) {
    processing = true;
    
    if(render_size.width() == 0 || render_size.height() == 0) return;
    
    RenderedCanvas rendered(render_size, range);
    QPainter painter(&rendered.get_image());
    CoordinateMapper mapper(render_size, range);
    
    if(painter.isActive() == false) {
        return;
    }
    
    QRect rect = mapper.map_to(DataRange(Timestamp(0), 0, data_thread->get_last_time(), 0)).toRect();
    rect.setTop(0);
    rect.setBottom(render_size.height() - 1);
    painter.setPen(Qt::NoPen);
    QBrush brush(Qt::lightGray);
    /*brush.setColor(qRgb(0, 240, 240));*/
    painter.setBrush(brush);
    painter.drawRect(rect);
    
    CanvasObject *object = canvas->get_head();
    while(object) {
        if(object->get_bounding_rect().intersects(range)) object->paint_onto(&painter, mapper);
        object = object->get_next();
    }
    
    processing = false;
    emit done(rendered);
}
