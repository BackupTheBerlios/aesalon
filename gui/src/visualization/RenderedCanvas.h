#ifndef AESALON_GUI_VISUALIZATION_RENDERED_CANVAS_H
#define AESALON_GUI_VISUALIZATION_RENDERED_CANVAS_H

#include <QImage>

#include "DataRange.h"

class RenderedCanvas {
private:
    QImage image;
    DataRange range;
public:
    RenderedCanvas();
    RenderedCanvas(QSize canvas_size, const DataRange &range);
    virtual ~RenderedCanvas();
    
    const QImage &get_image() const { return image; }
    QImage &get_image() { return image; }
    QSize get_size() const { return image.size(); }
    const DataRange &get_range() const { return range; }
    void set_range(const DataRange &new_range) { range = new_range; }
    
    void clear();
    void resize(QSize new_size);
    void shift(QPointF amount);
    void shift(DataPoint amount);
    void merge(RenderedCanvas &canvas);
};

Q_DECLARE_METATYPE(RenderedCanvas)

#endif
