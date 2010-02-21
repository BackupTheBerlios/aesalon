#include <QPainter>
#include "Visualization.h"
#include "Visualization.moc"

Visualization::Visualization(DataThread *data_thread, QWidget *parent)
    : QWidget(parent), v_thread(NULL), data_thread(data_thread), current_image(NULL) {
    
    main_layout = new QVBoxLayout();
    
    test_slider = new TimeSlider();
    main_layout->addWidget(test_slider);
    
    setLayout(main_layout);
}

Visualization::~Visualization() {
    if(v_thread) {
        v_thread->get_request_queue()->push_request(NULL);
        v_thread->wait();
    }
}

void Visualization::initialize() {
    v_thread = create_v_thread(data_thread);
    if(v_thread == NULL) {
        qDebug("Failed to create v_thread!");
        this->deleteLater();
        return;
    }
    connect(v_thread, SIGNAL(replace_image(QImage*)), SLOT(update_image(QImage*)));
    v_thread->start();
}

void Visualization::paintEvent(QPaintEvent *event) {
    /*QWidget::paintEvent(event);
    if(!current_image) return;
    QPainter painter(this);
    painter.scale(qreal(geometry().width()) / current_image->width(), qreal(geometry().height()) / current_image->height());
    painter.drawImage(0, 0, *current_image);*/
    QWidget::paintEvent(event);
}

void Visualization::update_image(QImage *image) {
    current_image = image;
}
