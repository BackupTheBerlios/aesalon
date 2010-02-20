#include "Session.h"
#include "Session.moc"
#include "SessionOverview.h"

Session::Session(QWidget *parent, DataSource *data_source) : QWidget(parent), data_source(data_source) {
    data_thread = new DataThread(this, data_source);
    data_thread->start();
    tab_widget = new QTabWidget();
    tab_widget->setTabPosition(QTabWidget::West);
    
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(tab_widget);
    setLayout(layout);
    
    SessionOverview *overview = new SessionOverview(data_thread);
    connect(data_thread, SIGNAL(started()), overview, SLOT(started()));
    connect(data_thread, SIGNAL(finished()), overview, SLOT(finished()));
    connect(overview, SIGNAL(new_visualization(Visualization*)), SLOT(new_visualization(Visualization*)));
    tab_widget->addTab(overview, tr("&Overview"));
}

Session::~Session() {
    data_thread->quit();
    data_thread->wait();
}

void Session::new_visualization(Visualization *visualization) {
    tab_widget->addTab(visualization, visualization->get_title());
}
