#include <QLayout>
#include <QSplitter>

#include "Visualization.h"
#include "Visualization.moc"
#include "CanvasGenerator.h"

Visualization::Visualization(VisualizationFactory *factory) {
    QVBoxLayout *main_layout = new QVBoxLayout();
    
    filter_manager = new FilterManager();
    
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
    
    request_button = new QPushButton(tr("&Actions"));
    upper_layout->addWidget(request_button);
    
    lock_box = new QCheckBox();
    lock_box->setEnabled(false);
    
    upper_layout->addWidget(lock_box);
    
    main_layout->addLayout(upper_layout);
    
    canvas = new Canvas(factory->get_vertical_axis_label());
    renderer = factory->create_renderer(canvas);
    CanvasGenerator *generator = new CanvasGenerator(factory->get_data_thread(), renderer);
    generator->exec();
    generator->deleteLater();
    
    factory->get_data_thread()->register_observer(renderer);
    
    QSplitter *splitter = new QSplitter;
    info_widget = new QWidget();
    info_widget->setMinimumWidth(200);
    info_widget->setMaximumWidth(600);
    info_widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    splitter->addWidget(info_widget);
    
    viewport = new Viewport(canvas, factory, info_widget, this);
    viewport->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(viewport, SIGNAL(mouse_position(QString)), SLOT(set_position(QString)), Qt::QueuedConnection);
    connect(viewport, SIGNAL(lock_change(bool)), lock_box, SLOT(setChecked(bool)));
    connect(cycle_time, SIGNAL(valueChanged(int)), viewport, SLOT(set_update_time(int)), Qt::QueuedConnection);
    
    splitter->addWidget(viewport);
    main_layout->addWidget(splitter);
    
    request_menu = new QMenu();
    QAction *action = new QAction(tr("&Attach"), request_menu);
    action->setCheckable(true);
    action->setChecked(false);
    connect(action, SIGNAL(triggered(bool)), viewport, SLOT(toggle_attach(bool)), Qt::QueuedConnection);
    request_menu->addAction(action);
    
    action = new QAction(tr("&Full view"), request_menu);
    connect(action, SIGNAL(triggered(bool)), viewport, SLOT(set_full_view()), Qt::QueuedConnection);
    request_menu->addAction(action);
    
    action = new QAction(tr("&Save screenshot"), request_menu);
    connect(action, SIGNAL(triggered(bool)), viewport, SLOT(save_screenshot()), Qt::QueuedConnection);
    request_menu->addAction(action);
    
    action = new QAction(tr("Manage &Filters"), request_menu);
    connect(action, SIGNAL(triggered(bool)), filter_manager, SLOT(show()), Qt::QueuedConnection);
    request_menu->addAction(action);
    
    request_button->setMenu(request_menu);
    
    setLayout(main_layout);
}

Visualization::~Visualization() {

}

void Visualization::set_position(QString formatted) {
    position_label->setText(formatted);
}
