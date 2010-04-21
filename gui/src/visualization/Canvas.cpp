#include <QPainter>
#include "Canvas.h"
#include "CoordinateMapper.h"

Canvas::Canvas() : head(NULL) {
}

Canvas::~Canvas() {
    this->clear();
}

DataRange Canvas::calculate_data_range() {
    if(!head) return DataRange();
    CanvasObject *object = head->get_next();
    if(!object) return DataRange();
    DataRange range;
    range.get_begin().set_data_element(object->get_bounding_rect().get_begin().get_data_element());
    range.get_end().set_data_element(object->get_bounding_rect().get_end().get_data_element());
    while(object) {
        if(object->get_bounding_rect().get_begin().get_data_element() < range.get_begin().get_data_element())
            range.get_begin().set_data_element(object->get_bounding_rect().get_begin().get_data_element());
        if(object->get_bounding_rect().get_end().get_data_element() > range.get_end().get_data_element())
            range.get_end().set_data_element(object->get_bounding_rect().get_end().get_data_element());
        
        object = object->get_next();
    }
    return range;
}

void Canvas::add_object(CanvasObject *object) {
    /*qDebug("Adding object to canvas, address is %p . . .", (void *)object);*/
    if(head == NULL) {
        head = object;
        head->set_next(NULL);
        head->set_prev(head);
    }
    else {
        head->get_prev()->set_next(object);
        head->set_prev(object);
    }
}

void Canvas::clear() {
    CanvasObject *object = head;
    while(object) {
        CanvasObject *next = object->get_next();
        delete object;
        object = next;
    }
}

CanvasObject *Canvas::object_at(const DataPoint &point) {
    CanvasObject *object = head;
    while(object) {
        if(object->get_bounding_rect().contains(point)) return object;
        object = object->get_next();
    }
    return NULL;
}

void Canvas::combine_with(const Canvas &canvas) {
    if(canvas.get_head() == NULL) return;
    CanvasObject *last = canvas.head->get_prev();
    if(head != NULL) {
        canvas.get_head()->set_prev(head->get_prev());
        head->get_prev()->set_next(canvas.head);
    }
    else {
        head = canvas.head;
    }
    head->set_prev(last);
}
