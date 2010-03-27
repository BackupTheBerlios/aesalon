#ifndef AESALON_GUI_VISUALIZATION_CANVAS_H
#define AESALON_GUI_VISUALIZATION_CANVAS_H

#include <QImage>

#include "DataRange.h"

class Canvas {
private:
    QImage image;
    DataRange range;
public:
    Canvas(const DataRange &range = DataRange());
    virtual ~Canvas();
    
    const DataRange &get_range() const { return range; }
    void set_range(const DataRange &new_range);
    QSize get_image_size() const { return image.size(); }
    void set_image_size(const QSize &new_size);
    
    void combine_with(const Canvas &canvas);
};

#endif
