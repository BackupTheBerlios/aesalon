#include <QPainter>
#include "Canvas.h"
#include "CoordinateMapper.h"

Canvas::Canvas(const DataRange &range) : range(range), head(NULL), insertion_point(NULL), termination_point(NULL) {
    
}

Canvas::~Canvas() {
    /*this->clear();*/
}

void Canvas::set_range(const DataRange &new_range) {
    range = new_range;
}

void Canvas::shift_range(const DataPoint &by) {
    range.get_begin().set_data_element(range.get_begin().get_data_element() + by.get_data_element());
    range.get_begin().set_time_element(Timestamp(range.get_begin().get_time_element().to_ns() + by.get_time_element().to_ns()));
    range.get_end().set_data_element(range.get_end().get_data_element() + by.get_data_element());
    range.get_end().set_time_element(Timestamp(range.get_end().get_time_element().to_ns() + by.get_time_element().to_ns()));
}

void Canvas::calculate_data_range() {
    if(!head) return;
    range.get_begin().set_data_element(head->get_bounding_rect().get_begin().get_data_element());
    range.get_end().set_data_element(head->get_bounding_rect().get_end().get_data_element());
    CanvasObject *object = head;
    while(object && object != termination_point) {
        if(object->get_bounding_rect().get_begin().get_data_element() < range.get_begin().get_data_element())
            range.get_begin().set_data_element(object->get_bounding_rect().get_begin().get_data_element());
        if(object->get_bounding_rect().get_end().get_data_element() > range.get_end().get_data_element())
            range.get_end().set_data_element(object->get_bounding_rect().get_end().get_data_element());
        
        object = object->get_next();
    }
}

void Canvas::add_object(CanvasObject *object) {
    /*qDebug("Adding object to canvas, address is %p . . .", (void *)object);*/
    if(head == NULL) {
        head = object;
        insertion_point = head;
    }
    else {
        insertion_point->set_next(object);
        insertion_point = object;
    }
}

void Canvas::clear() {
    /*if(termination_point == NULL) {
        qDebug("Decrementing refcount of objects in non-terminated canvas . . .");
        CanvasObject *object;
        CanvasObject *next = head;
        while((object = next) && object != termination_point) next = object->get_next(), object->dec_references();
    }*/
    head = NULL;
}

CanvasObject *Canvas::object_at(const DataPoint& point) {
    CanvasObject *object = head;
    while(object && object != termination_point) {
        if(object->get_bounding_rect().contains(point)) return object;
        object = object->get_next();
    }
    return NULL;
}

void Canvas::combine_with(const Canvas &canvas) {
    /*CanvasObject *object = canvas.head;
    while(object && object != termination_point) {
        if(object->get_bounding_rect().intersects(range)) {
            object->inc_references();
            add_object(object);
        }
        object = object->get_next();
    }*/
    if(termination_point != NULL || canvas.termination_point != NULL) {
        qWarning("Cannot merge with a canvas that has a termination point.");
        return;
    }
    if(insertion_point == NULL) head = canvas.head; 
    else insertion_point->set_next(canvas.head);
    
    insertion_point = canvas.insertion_point;
}

void Canvas::paint_onto(RenderedCanvas &canvas) {
    QPainter painter(&canvas.get_image());
    CoordinateMapper mapper(canvas.get_size(), range);
    CanvasObject *object = head;
    while(object && object != termination_point) {
        object->paint_onto(&painter, mapper);
        object = object->get_next();
    }
}

void Canvas::paint_onto(RenderedCanvas &canvas, const DataRange &range) {
    QPainter painter(&canvas.get_image());
    CoordinateMapper mapper(canvas.get_size(), range);
    CanvasObject *object = head;
    while(object && object != termination_point) {
        if(object->get_bounding_rect().intersects(range)) object->paint_onto(&painter, mapper);
        object = object->get_next();
    }
}

Canvas *Canvas::clone() const {
    Canvas *cloned = new Canvas(get_range());
    cloned->head = head;
    cloned->insertion_point = insertion_point;
    cloned->termination_point = insertion_point;
    return cloned;
}
