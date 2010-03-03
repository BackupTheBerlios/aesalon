#include <QPainter>
#include <QLabel>
#include <QScrollBar>
#include <QEvent>
#include <QWheelEvent>
#include "Visualization.h"
#include "Visualization.moc"

VisualizationCanvas::VisualizationCanvas(QWidget *parent) : QScrollArea(parent), image(NULL) {
    image_label = new QLabel(tr(". . ."));
    image_label->setMinimumSize(200, 200);
    image_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    image_label->setScaledContents(true);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setWidget(image_label);
    setWidgetResizable(true);
    scale = 1.0;
    
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(image_updated()));
    timer->start(5000);
    
    setMinimumSize(200, 200);
}

void VisualizationCanvas::wheelEvent(QWheelEvent* event) {
    scale *= 1.0 + (event->delta() / 1000.0);
    QTimer::singleShot(0, this, SLOT(image_updated()));
}

void VisualizationCanvas::mousePressEvent(QMouseEvent *event) {
    last_mouse_position = event->globalPos();
}

void VisualizationCanvas::mouseMoveEvent(QMouseEvent* event) {
    if(event->buttons() | Qt::LeftButton) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->globalPos().x() - last_mouse_position.x()));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->globalPos().y() - last_mouse_position.y()));
        last_mouse_position = event->globalPos();
    }
}

void VisualizationCanvas::update_image(QPixmap *image) {
    delete this->image;
    this->image = image;
    set_scale(1.0);
    this->update();
}

void VisualizationCanvas::image_updated() {
    if(image == NULL) return;
    image_label->setMinimumSize(image->width() * scale, image->height() * scale);
    image_label->setPixmap(*image);
}

void VisualizationCanvas::set_scale(qreal new_scale) {
    scale = new_scale;
}

Visualization::Visualization(DataThread *data_thread, QWidget *parent)
    : QWidget(parent), v_thread(NULL), data_thread(data_thread) {
    
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    
    main_layout = new QVBoxLayout();
    
    from_slider = new TimeSlider(tr("From:"));
    connect(from_slider, SIGNAL(changed(Timestamp)), SLOT(handle_slider_change_from(Timestamp)));
    main_layout->addWidget(from_slider);
    to_slider = new TimeSlider(tr("To:"));
    connect(to_slider, SIGNAL(changed(Timestamp)), SLOT(handle_slider_change_to(Timestamp)));
    main_layout->addWidget(to_slider);
    
    /*main_layout->addRow(tr("From:"), from_slider);
    main_layout->addRow(tr("To:"), to_slider);*/
    
    canvas = new VisualizationCanvas(this);
    canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    canvas->setMinimumSize(200, 200);
    main_layout->addWidget(canvas);
    /*main_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);*/
    
    setLayout(main_layout);
    current_request = NULL;
    
    QTimer *ranges_time = new QTimer(this);
    connect(ranges_time, SIGNAL(timeout()), SLOT(update_slider_ranges()));
    ranges_time->start(500);
}

Visualization::~Visualization() {
    if(v_thread) {
        v_thread->get_request_queue()->push_request(NULL);
        v_thread->quit();
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
    connect(v_thread, SIGNAL(replace_image(QPixmap*)), canvas, SLOT(update_image(QPixmap*)));
    connect(this, SIGNAL(visualization_request(VisualizationRequest*, QWidget*)), v_thread, SLOT(update_request(VisualizationRequest*, QWidget*)));
    connect(v_thread, SIGNAL(image_updated()), canvas, SLOT(image_updated()));
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
    /* NOTE: deleting this will crash the program if data is being visualized! */
    /*if(current_request) delete current_request;*/
    current_request = new VisualizationRequest(from_slider->current_value(), to_slider->current_value());
    qDebug("Emitting visualization_request(%p) . . .", (const void *)current_request);
    emit visualization_request(current_request, canvas);
}

void Visualization::handle_slider_change_to(Timestamp time) {
    if(time < from_slider->current_value()) {
        time = from_slider->current_value();
        time.add_ms(20);
        to_slider->set_value(time);
    }
    /* NOTE: deleting this will crash the program if data is being visualized! */
    /*if(current_request) delete current_request;*/
    current_request = new VisualizationRequest(from_slider->current_value(), to_slider->current_value());
    qDebug("Emitting visualization_request(%p) . . .", (const void *)current_request);
    qDebug("\tFrom %s to %s . . .", current_request->get_from().to_string().toStdString().c_str(), current_request->get_to().to_string().toStdString().c_str());
    qDebug("to_slider->current_value(): %s", to_slider->current_value().to_string().toStdString().c_str());
    emit visualization_request(current_request, canvas);
}
