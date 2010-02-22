#include <QPainter>
#include <QLabel>
#include "Visualization.h"
#include "Visualization.moc"

void VisualizationCanvas::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    if(!image) return;
    QPainter painter(this);
    painter.scale(qreal(geometry().width()) / image->width(), qreal(geometry().height()) / image->height());
    painter.drawImage(0, 0, *image);
}

void VisualizationCanvas::update_image(QImage *image) {
    this->image = image;
}


Visualization::Visualization(DataThread *data_thread, QWidget *parent)
    : QWidget(parent), v_thread(NULL), data_thread(data_thread) {
    
    main_layout = new QFormLayout();
    
    from_slider = new TimeSlider();
    connect(from_slider, SIGNAL(changed(Timestamp)), SLOT(handle_slider_change_from(Timestamp)));
    to_slider = new TimeSlider();
    connect(to_slider, SIGNAL(changed(Timestamp)), SLOT(handle_slider_change_to(Timestamp)));
    main_layout->addRow(tr("From:"), from_slider);
    main_layout->addRow(tr("To:"), to_slider);
    
    canvas = new VisualizationCanvas();
    canvas->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    main_layout->addWidget(canvas);
    
    setLayout(main_layout);
    current_request = NULL;
    
    QTimer *ranges_time = new QTimer(this);
    connect(ranges_time, SIGNAL(timeout()), SLOT(update_slider_ranges()));
    ranges_time->start(500);
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
    connect(v_thread, SIGNAL(replace_image(QImage*)), canvas, SLOT(update_image(QImage*)));
    connect(this, SIGNAL(visualization_request(VisualizationRequest*)), v_thread, SLOT(update_request(VisualizationRequest*)));
    v_thread->start();
    if(data_thread->get_start_time()) {
        from_slider->set_range(*data_thread->get_start_time(), Timestamp());
    }
}

void Visualization::update_slider_ranges() {
    if(data_thread->get_start_time()) {
        if(data_thread->get_finish_time()) {
            from_slider->set_range(*data_thread->get_start_time(), *data_thread->get_finish_time());
            to_slider->set_range(*data_thread->get_start_time(), *data_thread->get_finish_time());
        }
        else {
            from_slider->set_range(*data_thread->get_start_time(), Timestamp());
            to_slider->set_range(*data_thread->get_start_time(), Timestamp());
        }
    }
}

void Visualization::handle_slider_change_from(Timestamp time) {
    if(time > to_slider->current_value()) {
        time.add_ms(1);
        to_slider->set_value(time);
    }
    if(current_request) delete current_request;
    current_request = new VisualizationRequest(from_slider->current_value(), to_slider->current_value());
    qDebug("Emitting visualization_request(%p) . . .", (const void *)current_request);
    emit visualization_request(current_request);
}

void Visualization::handle_slider_change_to(Timestamp time) {
    if(time < from_slider->current_value()) {
        time = from_slider->current_value();
        time.add_ms(20);
        to_slider->set_value(time);
    }
    if(current_request) delete current_request;
    current_request = new VisualizationRequest(from_slider->current_value(), to_slider->current_value());
    qDebug("Emitting visualization_request(%p) . . .", (const void *)current_request);
    qDebug("\tFrom %s to %s . . .", current_request->get_from().to_string().toStdString().c_str(), current_request->get_to().to_string().toStdString().c_str());
    qDebug("to_slider->current_value(): %s", to_slider->current_value().to_string().toStdString().c_str());
    emit visualization_request(current_request);
}
