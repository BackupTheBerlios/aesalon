#ifndef AESALON_GUI_SESSION_VISUALIZATION_DISPLAY_H
#define AESALON_GUI_SESSION_VISUALIZATION_DISPLAY_H

#include <QGraphicsView>
#include <QWheelEvent>

#include "VisualizationCanvas.h"

class VisualizationDisplay : public QGraphicsView { Q_OBJECT
private:
    VisualizationCanvas *canvas;
    QPoint mouse_position;
public:
    VisualizationDisplay(QWidget *parent);
    virtual ~VisualizationDisplay();
    
    VisualizationCanvas *get_canvas() const { return canvas; };
public slots:
    void change_canvas(VisualizationCanvas *new_canvas);
protected:
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void paintEvent(QPaintEvent *event);
};

#endif
