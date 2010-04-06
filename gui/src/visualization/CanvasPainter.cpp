#include "CanvasPainter.h"
#include "CanvasPainter.moc"

CanvasPainter::CanvasPainter(): QThread(NULL) {

}

CanvasPainter::~CanvasPainter() {

}

void CanvasPainter::paint_canvas(QSize render_size, Canvas* canvas) {
    processing = true;
    RenderedCanvas rendered(render_size, canvas->get_range());
    canvas->paint_onto(rendered);
    processing = false;
    emit done(rendered);
}

void CanvasPainter::paint_canvas(QSize render_size, Canvas *canvas, DataRange range) {
    qDebug("CanvasPainter::paint_canvas(QSize, Canvas *, DataRange) called . . .");
    processing = true;
    RenderedCanvas rendered(render_size, range);
    canvas->paint_onto(rendered, range);
    processing = false;
    delete canvas;
    emit done(rendered);
}
