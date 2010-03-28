#include "Visualization.h"
#include "Visualization.moc"

Visualization::Visualization(Renderer *renderer, DataThread *data_thread) {
    QVBoxLayout *main_layout = new QVBoxLayout();
    
    upper_layout = new QHBoxLayout();
    position_label = new QLabel(tr("NYI"));
    upper_layout->addWidget(position_label, 1);
    
    cycle_time = new QSpinBox();
    cycle_time->setAlignment(Qt::AlignRight);
    cycle_time->setRange(100, 60000);
    cycle_time->setSingleStep(100);
    cycle_time->setPrefix(tr("Delay between real-time updates: "));
    cycle_time->setSuffix(tr("ms"));
    connect(cycle_time, SIGNAL(valueChanged(int)), SIGNAL(cycle_time_changed(int)));
    upper_layout->addWidget(cycle_time, 1);
    
    attach_button = new QPushButton(tr("&Attach"));
    connect(attach_button, SIGNAL(clicked(bool)), SIGNAL(attach_requested()));
    upper_layout->addWidget(attach_button);
    
    main_layout->addLayout(upper_layout);
    
    viewport = new Viewport(this);
    main_layout->addWidget(viewport);
    
    setLayout(main_layout);
    
    controller = new Controller(renderer, data_thread);
    controller->start();
    connect(this, SIGNAL(attach_requested()), controller, SLOT(something()));
    connect(this, SIGNAL(cycle_time_changed(int)), controller, SLOT(change_update_time(int)));
}

Visualization::~Visualization() {
    controller->quit();
    controller->wait();
}
