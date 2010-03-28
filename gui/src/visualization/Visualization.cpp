#include "Visualization.h"
#include "Visualization.moc"

Visualization::Visualization(VisualizationFactory *factory, DataThread *data_thread) {
    QVBoxLayout *main_layout = new QVBoxLayout();
    
    controller = new Controller(factory->create_renderer(), data_thread);
    connect(this, SIGNAL(cycle_time_changed(int)), controller, SLOT(change_update_time(int)));
    
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
    connect(cycle_time, SIGNAL(valueChanged(int)), SIGNAL(cycle_time_changed(int)));
    upper_layout->addWidget(cycle_time, 1);
    
    request_button = new QPushButton(tr("&Request"));
    request_menu = new QMenu();
    request_menu->addAction(tr("Real-&time visualization"), controller, SLOT(begin_rt()));
    request_menu->addAction(tr("&Full visualization"), controller, SLOT(render_full()));
    request_button->setMenu(request_menu);
    upper_layout->addWidget(request_button);
    
    main_layout->addLayout(upper_layout);
    
    viewport = new Viewport(factory, this);
    connect(controller, SIGNAL(canvas_update(Canvas*)), viewport, SLOT(merge_canvas(Canvas*)));
    connect(controller, SIGNAL(clear_canvas()), viewport, SLOT(clear_canvas()));
    connect(controller, SIGNAL(change_range(DataRange)), viewport, SLOT(set_canvas_range(DataRange)));
    connect(controller, SIGNAL(shift_range_to(Timestamp)), viewport, SLOT(shift_range_to(Timestamp)));
    connect(viewport, SIGNAL(mouse_position(QString)), SLOT(set_position(QString)));
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
