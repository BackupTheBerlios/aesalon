#include <QPainter>
#include "Visualization.h"
#include "Visualization.moc"

Visualization::Visualization(QWidget *parent) : QWidget(parent), current_image(NULL) {
    v_thread = create_v_thread();
    if(v_thread == NULL) {
        this->deleteLater();
        return;
    }
    connect(v_thread, SIGNAL(replace_image(QImage*)), SLOT(update_image(QImage*)));
    v_thread->start();
}

Visualization::~Visualization() {
    
}

void Visualization::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    if(!current_image) return;
    QPainter painter(this);
    painter.scale(qreal(geometry().width()) / current_image->width(), qreal(geometry().height()) / current_image->height());
    painter.drawImage(0, 0, *current_image);
}

void Visualization::update_image(QImage *image) {
    current_image = image;
}
