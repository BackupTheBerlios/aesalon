#ifndef AESALON_GUI_VISUALIZATION_CANVAS_OBJECT_H
#define AESALON_GUI_VISUALIZATION_CANVAS_OBJECT_H

#include <QPainter>
#include <QSet>

#include "DataRange.h"
#include "CoordinateMapper.h"

class Canvas;

class CanvasObject {
private:
    DataRange bounding_rect;
    CanvasObject *next;
    CanvasObject *prev;
public:
    CanvasObject(const DataRange &bounding_rect);
    virtual ~CanvasObject();
    
    const DataRange &get_bounding_rect() const { return bounding_rect; }
    
    virtual void paint_onto(QPainter *painter, const CoordinateMapper &mapper) = 0;
    
    CanvasObject *get_next() const { return next; }
    void set_next(CanvasObject *new_next) { next = new_next; }
    CanvasObject *get_prev() const { return prev; }
    void set_prev(CanvasObject *new_prev) { prev = new_prev; }
};

#endif
