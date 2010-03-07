#include "VisualizationDisplay.h"
#include "VisualizationDisplay.moc"

VisualizationDisplay::VisualizationDisplay(QWidget *parent): QGraphicsView(parent) {
    canvas = new VisualizationCanvas(this);
    setScene(canvas);
    QBrush bg(Qt::SolidPattern);
    bg.setColor(Qt::white);
    setBackgroundBrush(bg);
}

VisualizationDisplay::~VisualizationDisplay() {

}

void VisualizationDisplay::change_canvas(VisualizationCanvas *new_canvas) {
    canvas = new_canvas;
    this->setScene(canvas);
    this->update();
}

void VisualizationDisplay::wheelEvent(QWheelEvent *event) {
    qreal scale_amount = 1 + (event->delta() / 1000.0);
    scale(scale_amount, scale_amount);
    centerOn(mapToScene(event->globalPos()));
    QGraphicsView::wheelEvent(event);
}

void VisualizationDisplay::paintEvent(QPaintEvent *event) {
    QGraphicsView::paintEvent(event);
}
