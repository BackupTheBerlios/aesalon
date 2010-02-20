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
    
    SessionOverview *overview = new SessionOverview();
    connect(data_thread, SIGNAL(started()), overview, SLOT(started()));
    tab_widget->addTab(overview, tr("&Overview"));
}

Session::~Session() {
    data_thread->quit();
    data_thread->wait();
}
