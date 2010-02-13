#include <QPainter>
#include "Graph.h"
#include "Graph.moc"

Graph::Graph(QWidget *parent) : QWidget(parent) {
    graph_widget = new QWidget(this);
    graph_widget->setEnabled(false);
}

Graph::~Graph() {
    
}

void Graph::update_graph(QImage *image) {
    QPainter painter(graph_widget);
    
    painter.drawImage(0, 0, *image);
}
