#ifndef AESALON_GUI_SESSION_GRAPH_H
#define AESALON_GUI_SESSION_GRAPH_H

#include <QWidget>

class Graph : public QWidget { Q_OBJECT
public:
    Graph(QWidget *parent);
    virtual ~Graph();
};

#endif
