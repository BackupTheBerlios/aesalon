#include <QPainter>
#include "Canvas.h"

Canvas::Canvas(const DataRange &range) : image(200, 200, QImage::Format_RGB32), range(range) {
    
}

Canvas::~Canvas() {
    
}

void Canvas::set_range(const DataRange &new_range) {
    range = new_range;
}

void Canvas::set_image_size(const QSize &new_size) {
    image = image.scaled(new_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void Canvas::combine_with(const Canvas &canvas) {
    /* If the canvases have no common range, just return . . . */
    if(!range.intersects(canvas.range)) return;
    DataRange intersecting_range = range.intersecting_range(canvas.range);
    
    qreal canvas_x_ratio = canvas.range.get_begin().get_time_element().ns_until(canvas.range.get_end().get_time_element()) / canvas.image.width();
    qreal canvas_y_ratio = (canvas.range.get_end().get_data_element() - canvas.range.get_begin().get_data_element()) / canvas.image.width();
    qreal x_ratio = range.get_begin().get_time_element().ns_until(range.get_end().get_time_element()) / image.width();
    qreal y_ratio = (range.get_end().get_data_element() - range.get_begin().get_data_element()) / image.width();
    
    QImage other_scaled = canvas.image.scaled((x_ratio / canvas_x_ratio) * canvas.image.width(), (y_ratio / canvas_y_ratio) * canvas.image.height());
}

void Canvas::paint_onto(QPaintDevice* device) {
    QPainter painter(device);
    QSize device_size(device->width(), device->height());
    set_image_size(device_size);
    painter.drawImage(0, 0, image);
}
