#include "Viewport.h"
#include "Viewport.moc"

Viewport::Viewport(QWidget *parent): QWidget(parent) {

}

Viewport::~Viewport() {

}

void Viewport::merge_canvas(Canvas* canvas) {
    local_canvas.combine_with(*canvas);
}

void Viewport::clear_canvas() {
    local_canvas.clear();
}
