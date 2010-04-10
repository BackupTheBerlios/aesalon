#include "Visualization.h"
#include "Visualization.moc"

Visualization::Visualization(VisualizationFactory *factory) {
    QVBoxLayout *main_layout = new QVBoxLayout();
    
    upper_layout = new QHBoxLayout();
    position_label = new QLabel(tr("NYI"));
    upper_layout->addWidget(position_label, 1);
    
    cycle_time = new QSpinBox();
    cycle_time->setAlignment(Qt::AlignRight);
    cycle_time->setRange(100, 60000);
    cycle_time->setSingleStep(100);
    cycle_time->setValue(1000);
    cycle_time->setPrefix(tr("Delay between real-time updates: "));
    cycle_time->setSuffix(tr("ms"));
    connect(cycle_time, SIGNAL(valueChanged(int)), SIGNAL(cycle_time_changed(int)), Qt::QueuedConnection);
    upper_layout->addWidget(cycle_time, 1);
    
    request_button = new QPushButton(tr("&Request"));
    request_menu = new QMenu();
    request_menu->addAction(tr("Real-&time visualization"), this, SLOT(send_begin_rt()));
    request_menu->addAction(tr("&Stop real-time updating"), this, SLOT(send_end_rt()));
    request_menu->addAction(tr("&Full visualization"), this, SLOT(send_render_full()));
    
    request_button->setMenu(request_menu);
    upper_layout->addWidget(request_button);
    
    main_layout->addLayout(upper_layout);
    
    controller = new Controller(factory);
    connect(this, SIGNAL(cycle_time_changed(int)), controller, SLOT(change_update_time(int)));
    viewport = new Viewport(factory, this);
    connect(this, SIGNAL(begin_rt()), controller, SLOT(begin_rt()), Qt::QueuedConnection);
    connect(this, SIGNAL(end_rt()), controller, SLOT(end_rt()), Qt::QueuedConnection);
    connect(this, SIGNAL(render_full()), controller, SLOT(render_full()), Qt::QueuedConnection);
    connect(controller, SIGNAL(canvas_update(Canvas*)), viewport, SLOT(merge_canvas(Canvas*)), Qt::QueuedConnection);
    connect(controller, SIGNAL(clear_canvas()), viewport, SLOT(clear_canvas()), Qt::QueuedConnection);
    connect(controller, SIGNAL(change_range(DataRange)), viewport, SLOT(set_canvas_range(DataRange)), Qt::QueuedConnection);
    connect(controller, SIGNAL(shift_range_to(Timestamp)), viewport, SLOT(shift_range_to(Timestamp)), Qt::QueuedConnection);
    connect(controller, SIGNAL(force_repaint()), viewport, SLOT(force_render()), Qt::QueuedConnection);
    connect(viewport, SIGNAL(mouse_position(QString)), SLOT(set_position(QString)), Qt::QueuedConnection);
    main_layout->addWidget(viewport);
    
    setLayout(main_layout);
    
    controller->start();
}

Visualization::~Visualization() {
    controller->quit();
    controller->wait();
}

void Visualization::set_position(QString formatted) {
    position_label->setText(formatted);
}

void Visualization::send_begin_rt() {
    emit begin_rt();
}

void Visualization::send_end_rt() {
    emit end_rt();
}

void Visualization::send_render_full() {
    emit render_full();
}
