#include <QPainter>
#include "GraphWidget.h"
#include "GraphWidget.moc"

GraphWidget::GraphWidget(QWidget *parent) : QWidget(parent), current_image(NULL) {
    
}

GraphWidget::~GraphWidget() {
    
}

void GraphWidget::set_data_engine(GraphDataEngine *data_engine) {
    this->data_engine = data_engine;
}

void GraphWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    qreal x_scale = 0, y_scale = 0;
    x_scale = qreal(geometry().width()) / current_image->width();
    y_scale = qreal(geometry().height()) / current_image->height();
    painter.scale(x_scale, y_scale);
    painter.drawImage(0, 0, *current_image);
}

void GraphWidget::update_image(QImage *new_image) {
    current_image = new_image;
}
