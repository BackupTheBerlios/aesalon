#include <QPainter>
#include <QLabel>
#include "Visualization.h"
#include "Visualization.moc"

Visualization::Visualization(DataThread *data_thread, QWidget *parent)
    : QWidget(parent), v_thread(NULL), data_thread(data_thread), current_image(NULL) {
    
    main_layout = new QVBoxLayout();
    
    from_slider = new TimeSlider();
    connect(from_slider, SIGNAL(changed(Timestamp)), SLOT(handle_slider_change_from(Timestamp)));
    to_slider = new TimeSlider();
    connect(to_slider, SIGNAL(changed(Timestamp)), SLOT(handle_slider_change_to(Timestamp)));
    main_layout->addWidget(new QLabel(tr("From:")));
    main_layout->addWidget(from_slider);
    main_layout->addWidget(new QLabel(tr("To:")));
    main_layout->addWidget(to_slider);
    main_layout->addStretch(1);
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

void Visualization::handle_slider_change_from(Timestamp time) {
    if(time > to_slider->current_value()) {
        time.add_ms(1);
        to_slider->set_value(time);
    }
}

void Visualization::handle_slider_change_to(Timestamp time) {
    if(time < from_slider->current_value()) {
        time = from_slider->current_value();
        time.add_ms(1);
        to_slider->set_value(time);
    }
}
