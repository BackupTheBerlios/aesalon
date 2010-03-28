#ifndef AESALON_VISUALIZATION_VIEWPORT_H
#define AESALON_VISUALIZATION_VIEWPORT_H

#include <QWidget>

#include "Canvas.h"
#include "CanvasPainter.h"
#include "VisualizationFactory.h"

class Viewport : public QWidget { Q_OBJECT
private:
    Canvas local_canvas;
    CanvasPainter *canvas_painter;
    QImage rendered;
    AxisFormatter *formatter;
public:
    Viewport(VisualizationFactory *factory, QWidget *parent);
    virtual ~Viewport();
public slots:
    void merge_canvas(Canvas *canvas);
    void clear_canvas();
    void set_canvas_range(const DataRange &new_range);
    void shift_range_to(const Timestamp &high_time);
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
signals:
    void paint_canvas(Canvas *canvas);
    void mouse_position(QString formatted);
};

#endif
