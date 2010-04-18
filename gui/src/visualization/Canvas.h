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
    QList<DataRange> ranges;
public:
    Canvas();
    virtual ~Canvas();
    
    DataRange calculate_data_range();
    
    void add_object(CanvasObject *object);
    void clear();
    CanvasObject *object_at(const DataPoint &point);
    
    QList<DataRange> take_updated_ranges() { QList<DataRange> ranges = this->ranges; this->ranges.clear(); return ranges; }
    void add_updated_range(const DataRange &range) { ranges.append(range); }
    void clear_updated_ranges() { ranges.clear(); }
    
    void combine_with(const Canvas &canvas);
    
    CanvasObject *get_head() const { return head; }
    void set_head(CanvasObject *new_head) { head = new_head; }
};

#endif
