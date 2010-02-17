#ifndef AESALON_GUI_SESSION_GRAPH_WIDGET_H
#define AESALON_GUI_SESSION_GRAPH_WIDGET_H

#include <QWidget>

#include "GraphDataEngine.h"

class GraphWidget : public QWidget { Q_OBJECT
private:
    GraphDataEngine *data_engine;
    QImage *current_image;
public:
    GraphWidget(QWidget *parent);
    virtual ~GraphWidget();
protected:
    virtual void paintEvent(QPaintEvent *event);
public slots:
    void set_data_engine(GraphDataEngine *data_engine);
    void update_image(QImage *new_image);
};

#endif
