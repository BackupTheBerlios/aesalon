#include "CanvasPainter.h"
#include "CanvasPainter.moc"

CanvasPainter::CanvasPainter(QImage* image): QThread(NULL), image(image) {

}

CanvasPainter::~CanvasPainter() {

}

void CanvasPainter::paint_canvas(Canvas* canvas) {
    image->fill(qRgb(255, 255, 255));
    canvas->paint_onto(image);
    emit done();
}

void CanvasPainter::paint_canvas(Canvas *canvas, DataRange range) {
    CoordinateMapper mapper(image->size(), canvas->get_range());
    QPainter painter(image);
    painter.drawRect(mapper.map_to(range));
    painter.end();
    canvas->paint_onto(image, range);
    emit done();
}
