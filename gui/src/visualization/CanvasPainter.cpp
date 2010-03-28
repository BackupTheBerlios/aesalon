#include "CanvasPainter.h"
#include "CanvasPainter.moc"

CanvasPainter::CanvasPainter(QImage* image): QThread(NULL), image(image) {

}

CanvasPainter::~CanvasPainter() {

}

void CanvasPainter::paint_canvas(Canvas* canvas) {
    qDebug("CanvasPainter::paint_canvas(): image address is %p", image);
    image->fill(qRgb(255, 255, 255));
    canvas->paint_onto(image);
}
