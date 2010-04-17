#ifndef AESALON_GUI_VISUALIZATION_CANVAS_H
#define AESALON_GUI_VISUALIZATION_CANVAS_H

#include <QImage>

#include "DataRange.h"
#include "CanvasObject.h"
#include "RenderedCanvas.h"

class Canvas {
private:
    DataRange range;
    CanvasObject *head;
    CanvasObject *termination_point;
    
    int object_count;
public:
    Canvas(const DataRange &range = DataRange());
    virtual ~Canvas();
    
    const DataRange &get_range() const { return range; }
    void set_range(const DataRange &new_range);
    void shift_range(const DataPoint &by);
    void calculate_data_range();
    
    void add_object(CanvasObject *object);
    void clear();
    CanvasObject *object_at(const DataPoint &point);
    
    void combine_with(const Canvas &canvas);
    
    
    CanvasObject *get_head() const { return head; }
    void set_head(CanvasObject *new_head) { head = new_head; }
    CanvasObject *get_termination_point() const { return termination_point; }
    /**
        @deprecated
    */
    void paint_onto(RenderedCanvas &canvas);
    /**
        @deprecated
    */
    void paint_onto(RenderedCanvas &canvas, const DataRange &range);
    
    Canvas *clone() const;
    
    void inc_object_count(int amount = 1) { object_count += amount; }
    void dec_object_count(int amount = 1) { object_count -= amount; }
    int get_object_count() const { return object_count; }
};

#endif
