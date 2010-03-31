#include <QPainter>
#include "Canvas.h"
#include "CoordinateMapper.h"

Canvas::Canvas(const DataRange &range) : range(range) {
    
}

Canvas::~Canvas() {
    this->clear();
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
    if(!objects.size()) return;
    range.get_begin().set_data_element(objects[0]->get_bounding_rect().get_begin().get_data_element());
    range.get_end().set_data_element(objects[0]->get_bounding_rect().get_end().get_data_element());
    foreach(CanvasObject *object, objects) {
        if(object->get_bounding_rect().get_begin().get_data_element() < range.get_begin().get_data_element())
            range.get_begin().set_data_element(object->get_bounding_rect().get_begin().get_data_element());
        if(object->get_bounding_rect().get_end().get_data_element() > range.get_end().get_data_element())
            range.get_end().set_data_element(object->get_bounding_rect().get_end().get_data_element());
    }
}

void Canvas::add_object(CanvasObject *object) {
    objects.append(object);
    object->inc_references();
}

void Canvas::clear() {
    foreach(CanvasObject *object, objects) {
        object->dec_references();
    }
    objects.clear();
}

void Canvas::combine_with(const Canvas &canvas) {
    foreach(CanvasObject *object, canvas.objects) {
        if(object->get_bounding_rect().intersects(range)) {
            objects.append(object);
            object->inc_references();
        }
    }
}

void Canvas::paint_onto(QPaintDevice* device) {
    QPainter painter(device);
    QSize device_size(device->width(), device->height());
    CoordinateMapper mapper(device_size, range);
    foreach(CanvasObject *object, objects) {
        object->paint_onto(&painter, mapper);
    }
}
