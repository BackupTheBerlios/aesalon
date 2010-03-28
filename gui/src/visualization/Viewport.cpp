#include <QWhatsThis>
#include <QMouseEvent>
#include <QTimer>
#include "Viewport.h"
#include "Viewport.moc"

Viewport::Viewport(QWidget *parent): QWidget(parent) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(update()));
    timer->start(500);
    canvas_painter = new CanvasPainter(&rendered);
    rendered = QImage(width(), height(), QImage::Format_RGB32);
    
    connect(this, SIGNAL(paint_canvas(Canvas *)), canvas_painter, SLOT(paint_canvas(Canvas*)));
}

Viewport::~Viewport() {

}

void Viewport::merge_canvas(Canvas* canvas) {
    local_canvas.combine_with(*canvas);
    emit paint_canvas(&local_canvas);
}

void Viewport::clear_canvas() {
    local_canvas.clear();
}

void Viewport::set_canvas_range(const DataRange &new_range) {
    local_canvas.set_range(new_range);
}

void Viewport::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(0, 0, rendered);
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
    
}

void Viewport::resizeEvent(QResizeEvent *event) {
    rendered = rendered.scaled(event->size());
}
