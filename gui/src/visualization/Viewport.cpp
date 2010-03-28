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
}

Viewport::~Viewport() {

}

void Viewport::merge_canvas(Canvas* canvas) {
    local_canvas.combine_with(*canvas);
}

void Viewport::clear_canvas() {
    local_canvas.clear();
}

void Viewport::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), qRgb(qrand() % 256, qrand() % 256, qrand() % 256));
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
    
}
