#ifndef AESALON_GUI_VISUALIZATION_CANVAS_H
#define AESALON_GUI_VISUALIZATION_CANVAS_H

#include <QImage>

#include "DataRange.h"
#include "CanvasObject.h"
#include "RenderedCanvas.h"

#include "storage/EventVisitor.h"

class Canvas {
private:
    CanvasObject *head;
public:
    Canvas();
    virtual ~Canvas();
    
    DataRange calculate_data_range();
    
    void add_object(CanvasObject *object);
    void clear();
    CanvasObject *object_at(const DataPoint &point);
    
    void combine_with(const Canvas &canvas);
    
    CanvasObject *get_head() const { return head; }
    void set_head(CanvasObject *new_head) { head = new_head; }
};

#endif
