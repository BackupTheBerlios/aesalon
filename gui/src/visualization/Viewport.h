#ifndef AESALON_VISUALIZATION_VIEWPORT_H
#define AESALON_VISUALIZATION_VIEWPORT_H

#include <QWidget>

#include "Canvas.h"
#include "CanvasPainter.h"
#include "VisualizationFactory.h"
#include "ClickHandler.h"
#include "RenderedCanvas.h"

class Viewport : public QWidget { Q_OBJECT
private:
    Canvas *canvas;
    CanvasPainter *canvas_painter;
    RenderedCanvas rendered_canvas;
    AxisFormatter *formatter;
    ClickHandler *click_handler;
    QPointF old_mouse_pos;
    DataThread *data_thread;
public:
    Viewport(Canvas *canvas, VisualizationFactory *factory, QWidget *parent);
    virtual ~Viewport();
public slots:
    void set_canvas_range(const DataRange &new_range);
    void shift_range_to(const Timestamp &high_time);
    void force_render();
    void set_full_view();
private slots:
    void merge_canvas(RenderedCanvas canvas);
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
signals:
    void paint_canvas(QSize render_size, Canvas *canvas);
    void paint_canvas(QSize render_size, Canvas *canvas, DataRange range);
    void mouse_position(QString formatted);
};

#endif
