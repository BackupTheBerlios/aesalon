#ifndef AESALON_GUI_VISUALIZATION_CANVAS_H
#define AESALON_GUI_VISUALIZATION_CANVAS_H

#include <QImage>

#include "DataRange.h"
#include "CanvasObject.h"

class Canvas {
private:
    DataRange range;
    QList<CanvasObject *> objects;
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
    
    void paint_onto(QPaintDevice *device);
    void paint_onto(QPaintDevice *device, const DataRange &range);
};

#endif
