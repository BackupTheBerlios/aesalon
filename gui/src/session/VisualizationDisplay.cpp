#include <QScrollBar>
#include "VisualizationDisplay.h"
#include "VisualizationDisplay.moc"

VisualizationDisplay::VisualizationDisplay(QWidget *parent): QGraphicsView(parent) {
    canvas = new VisualizationCanvas(this);
    setScene(canvas);
    QBrush bg(Qt::SolidPattern);
    bg.setColor(Qt::white);
    setBackgroundBrush(bg);
    position_label = new QLabel();
    this->setCornerWidget(position_label);
    this->setCursor(Qt::CrossCursor);
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
    centerOn(mapToScene(event->pos()));
    QGraphicsView::wheelEvent(event);
}

void VisualizationDisplay::mouseMoveEvent(QMouseEvent* event) {
    if(event->buttons() & Qt::LeftButton) {
        QPoint difference = mouse_position - event->globalPos();
        
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + difference.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() + difference.y());
        
        mouse_position = event->globalPos();
    }
    if(scene()->height()) {
        QPointF where = mapToScene(event->pos());
        where.setY(scene()->height() - where.y());
        qreal value_percentage = where.y() / scene()->height();
        qreal timestamp_percentage = where.x() / scene()->width();
        qint64 total_ms = canvas->get_data_range().get_lower_time().ms_until(canvas->get_data_range().get_upper_time());
        qint64 total_data_range = canvas->get_data_range().get_upper_data() - canvas->get_data_range().get_lower_data();
        qint64 ms = (timestamp_percentage * total_ms);
        emit position_changed(ms, (value_percentage * total_data_range) + canvas->get_data_range().get_lower_data());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void VisualizationDisplay::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton)
        mouse_position = event->globalPos();
    QGraphicsView::mousePressEvent(event);
}

void VisualizationDisplay::paintEvent(QPaintEvent *event) {
    QGraphicsView::paintEvent(event);
}

void VisualizationDisplay::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
}

void VisualizationDisplay::keyReleaseEvent(QKeyEvent *event) {
    QGraphicsView::keyReleaseEvent(event);
}

void VisualizationDisplay::resizeEvent(QResizeEvent* event) {
    emit update_request();
    QGraphicsView::resizeEvent(event);
}
