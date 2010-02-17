#include <QPainter>
#include "Session.h"
#include "Session.moc"

Session::Session(QWidget *parent, DataSource *data_source) : QWidget(parent), data_source(data_source) {
    data_thread = new DataThread(this, data_source);
    data_thread->start();
    
    primary_layout = new QVBoxLayout();
    
    QPushButton *add_graph_widget = new QPushButton(tr("&New Graph"));
    connect(add_graph_widget, SIGNAL(clicked()), this, SLOT(create_graph_widget()));
    primary_layout->addWidget(add_graph_widget);
    tab_bar = new QTabWidget();
    primary_layout->addWidget(tab_bar);
    
    setLayout(primary_layout);
}

Session::~Session() {
    data_thread->quit();
    data_thread->wait();
}

void Session::create_graph_widget() {
    GraphWidget *graph_widget = new GraphWidget(NULL);
    tab_bar->addTab(graph_widget, "Graph");
    QImage *image = new QImage(QSize(300, 300), QImage::Format_RGB32);
    image->fill(QColor(255, 255, 255).rgb());
    QPainter painter(image);
    graph_widget->update_image(image);
}
