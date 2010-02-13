#ifndef AESALON_GUI_SESSION_GRAPH_H
#define AESALON_GUI_SESSION_GRAPH_H

#include <QMap>
#include <QWidget>
#include <QImage>

#include "GraphData.h"
#include "storage/Timestamp.h"

class Graph : public QWidget { Q_OBJECT
private:
    QWidget *graph_widget;
public:
    Graph(QWidget *parent);
    virtual ~Graph();
public slots:
    void update_graph(QImage *new_image);
};

#endif
