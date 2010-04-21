#ifndef AESALON_GUI_VISUALIZATION_CANVAS_PAINTER_H
#define AESALON_GUI_VISUALIZATION_CANVAS_PAINTER_H

#include <QImage>
#include <QThread>

#include "Canvas.h"
#include "RenderedCanvas.h"
#include "CanvasMap.h"
#include "session/DataThread.h"

class CanvasPainter : public QThread { Q_OBJECT
private:
    bool processing;
    DataThread *data_thread;
public:
    CanvasPainter(DataThread *data_thread);
    virtual ~CanvasPainter();
    
    bool is_processing() const { return processing; }
    
public slots:
    void paint_canvas(QSize render_size, Canvas *canvas);
    void paint_canvas(QSize render_size, Canvas *canvas, DataRange range);
signals:
    void done(RenderedCanvas rendered);
};

#endif
