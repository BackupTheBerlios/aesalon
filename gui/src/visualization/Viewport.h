#ifndef AESALON_VISUALIZATION_VIEWPORT_H
#define AESALON_VISUALIZATION_VIEWPORT_H

#include <QWidget>

#include "Canvas.h"
#include "CanvasPainter.h"

class Viewport : public QWidget { Q_OBJECT
private:
    Canvas local_canvas;
    CanvasPainter *canvas_painter;
    QImage rendered;
public:
    Viewport(QWidget *parent);
    virtual ~Viewport();
public slots:
    void merge_canvas(Canvas *canvas);
    void clear_canvas();
    void set_canvas_range(const DataRange &new_range);
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
signals:
    void paint_canvas(Canvas *canvas);
};

#endif
