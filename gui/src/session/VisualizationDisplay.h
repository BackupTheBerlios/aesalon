#ifndef AESALON_GUI_SESSION_VISUALIZATION_DISPLAY_H
#define AESALON_GUI_SESSION_VISUALIZATION_DISPLAY_H

#include <QLabel>
#include <QGraphicsView>
#include <QWheelEvent>

#include "VisualizationCanvas.h"

class VisualizationDisplay : public QGraphicsView { Q_OBJECT
private:
    VisualizationCanvas *canvas;
    QPoint mouse_position;
    QLabel *position_label;
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
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
signals:
    void position_changed(qint64 time, qreal value);
    void update_request();
};

#endif
