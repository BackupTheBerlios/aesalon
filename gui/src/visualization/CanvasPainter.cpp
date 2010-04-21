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
    object->paint_onto(&painter, mapper);
    object = object->get_next();
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
    object->paint_onto(&painter, mapper);
    object = object->get_next();
    while(object) {
        if(object->get_bounding_rect().intersects(range)) object->paint_onto(&painter, mapper);
        object = object->get_next();
    }
    
    processing = false;
    emit done(rendered);
}
