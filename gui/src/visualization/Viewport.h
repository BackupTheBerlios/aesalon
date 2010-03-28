#ifndef AESALON_VISUALIZATION_VIEWPORT_H
#define AESALON_VISUALIZATION_VIEWPORT_H

#include <QWidget>

#include "Canvas.h"

class Viewport : public QWidget { Q_OBJECT
private:
    Canvas local_canvas;
public:
    Viewport(QWidget *parent);
    virtual ~Viewport();
public slots:
    void merge_canvas(Canvas *canvas);
    void clear_canvas();
};

#endif
