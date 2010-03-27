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
    
}
