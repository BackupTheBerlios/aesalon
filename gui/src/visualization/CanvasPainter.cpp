#include "CanvasPainter.h"
#include "CanvasPainter.moc"

CanvasPainter::CanvasPainter(): QThread(NULL) {
    last_map = &maps[0];
    new_map = &maps[1];
}

CanvasPainter::~CanvasPainter() {

}

void CanvasPainter::paint_canvas(QSize render_size, Canvas *canvas) {
    qDebug("painting, canvas object count is %i", canvas->get_object_count());
    processing = true;
    
    RenderedCanvas rendered(render_size, canvas->get_range());
    QPainter painter(&rendered.get_image());
    CoordinateMapper mapper(render_size, canvas->get_range());
    
    new_map->clear();
    
    CanvasObject *object, *termination_point;
    object = canvas->get_head();
    termination_point = canvas->get_termination_point();
    while(object) {
        object->paint_onto(&painter, mapper);
#if 0
        qreal address = object->get_bounding_rect().get_begin().get_data_element();
        if(canvas->get_head() != NULL) {
            CanvasObject *last_object = last_map->value(address);
            if(last_object && last_object != object) {
                qDebug("Removing duplicate object . . .");
                /* This will only occur if this is the first element . . . */
                if(last_object->get_prev()->get_next() == NULL) {
                    qDebug("Removing head . . .");
                    canvas->set_head(last_object->get_next());
                    if(canvas->get_head()) canvas->get_head()->set_prev(last_object->get_prev());
                }
                else {
                    qDebug("Removing normal object . . .");
                    last_object->get_prev()->set_next(last_object->get_next());
                    if(last_object->get_next()) {
                        last_object->get_next()->set_prev(last_object->get_prev());
                    }
                }
                canvas->dec_object_count();
            }
        }
        new_map->insert(address, object);
#endif
        
        if(object == termination_point) break;
        object = object->get_next();
        if(object == canvas->get_head()) qDebug("Closed doubly-linked list . . .");
    }
    processing = false;
    emit done(rendered);
    CanvasMap *temp = last_map;
    last_map = new_map;
    new_map = temp;
}

void CanvasPainter::paint_canvas(QSize render_size, Canvas *canvas, DataRange range) {
    qDebug("painting, canvas object count is %i", canvas->get_object_count());
    processing = true;
    
    RenderedCanvas rendered(render_size, range);
    QPainter painter(&rendered.get_image());
    CoordinateMapper mapper(render_size, range);
    
    new_map->clear();
    if(canvas->get_head() == NULL) last_map->clear();
    
    CanvasObject *object, *termination_point;
    object = canvas->get_head();
    termination_point = canvas->get_termination_point();
    while(object) {
        if(object->get_bounding_rect().intersects(range)) object->paint_onto(&painter, mapper);
#if 0
        qreal address = object->get_bounding_rect().get_begin().get_data_element();
        CanvasObject *last_object = last_map->value(address);
        if(last_object && last_object != object) {
            qDebug("Removing duplicate object . . .");
            /* This will only occur if this is the first element . . . */
            if(last_object->get_prev()->get_next() == NULL) {
                canvas->set_head(last_object->get_next());
            }
            else {
                last_object->get_prev()->set_next(last_object->get_next());
                if(last_object->get_next()) {
                    last_object->get_next()->set_prev(last_object->get_prev());
                }
            }
            canvas->dec_object_count();
        }
        new_map->insert(address, object);
#endif
        if(object == termination_point) break;
        object = object->get_next();
    }
    processing = false;
    emit done(rendered);
    
    CanvasMap *temp = last_map;
    last_map = new_map;
    new_map = temp;
}
