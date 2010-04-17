#include "CanvasPainter.h"
#include "CanvasPainter.moc"

CanvasPainter::CanvasPainter(): QThread(NULL) {
    last_map = &maps[0];
    new_map = &maps[1];
}

CanvasPainter::~CanvasPainter() {

}

void CanvasPainter::paint_canvas(QSize render_size, Canvas *canvas) {
    processing = true;
    
    RenderedCanvas rendered(render_size, canvas->get_range());
    QPainter painter(&rendered.get_image());
    CoordinateMapper mapper(render_size, canvas->get_range());
    
    CanvasObject *object, *termination_point;
    object = canvas->get_head();
    termination_point = canvas->get_termination_point();
    while(object) {
        object->paint_onto(&painter, mapper);
        if(object == termination_point) break;
        object = object->get_next();
    }
    processing = false;
    emit done(rendered);
}

void CanvasPainter::paint_canvas(QSize render_size, Canvas *canvas, DataRange range) {
    processing = true;
    
    RenderedCanvas rendered(render_size, range);
    QPainter painter(&rendered.get_image());
    CoordinateMapper mapper(render_size, range);
    
    CanvasObject *object, *termination_point;
    object = canvas->get_head();
    termination_point = canvas->get_termination_point();
    while(object) {
        /*if(object->get_bounding_rect().intersects(range)) */object->paint_onto(&painter, mapper);
        if(object == termination_point) break;
        object = object->get_next();
    }
    processing = false;
    emit done(rendered);
}
