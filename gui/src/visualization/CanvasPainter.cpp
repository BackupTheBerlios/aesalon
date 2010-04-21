#include "CanvasPainter.h"
#include "CanvasPainter.moc"

CanvasPainter::CanvasPainter(): QThread(NULL) {
}

CanvasPainter::~CanvasPainter() {

}

void CanvasPainter::paint_canvas(QSize render_size, Canvas *canvas) {
    processing = true;
    
    DataRange range = canvas->calculate_data_range();
    RenderedCanvas rendered(render_size, range);
    QPainter painter(&rendered.get_image());
    CoordinateMapper mapper(render_size, range);
    
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
    
    CanvasObject *object;
    object = canvas->get_head();
    while(object) {
        qDebug("object address: %p", object);
        qDebug("object bounding rectangle: (%lli, %f),(%lli, %f)",
            object->get_bounding_rect().get_begin().get_time_element().to_ns(),
            object->get_bounding_rect().get_begin().get_data_element(),
            object->get_bounding_rect().get_end().get_time_element().to_ns(),
            object->get_bounding_rect().get_end().get_data_element());
        if(object->get_bounding_rect().intersects(range)) object->paint_onto(&painter, mapper);
        object = object->get_next();
    }
    
    processing = false;
    emit done(rendered);
}
