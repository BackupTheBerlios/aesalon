#ifndef AESALON_GUI_SESSION_GRAPH_H
#define AESALON_GUI_SESSION_GRAPH_H

#include <QWidget>

#include "GraphData.h"
#include "storage/Timestamp.h"

class Graph : public QWidget { Q_OBJECT
private:
    QMap<Timestamp, GraphData *> graph_data;
public:
    Graph(QWidget *parent);
    virtual ~Graph();
};

#endif
