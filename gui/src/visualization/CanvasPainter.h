#ifndef AESALON_GUI_VISUALIZATION_CANVAS_PAINTER_H
#define AESALON_GUI_VISUALIZATION_CANVAS_PAINTER_H

#include <QImage>
#include <QThread>

#include "Canvas.h"

class CanvasPainter : public QThread { Q_OBJECT
private:
    QImage *image;
public:
    CanvasPainter(QImage *image);
    virtual ~CanvasPainter();
    
public slots:
    void paint_canvas(Canvas *canvas);
signals:
    void done();
};

#endif
