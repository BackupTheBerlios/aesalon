#include <QWhatsThis>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include "Viewport.h"
#include "Viewport.moc"

Viewport::Viewport(VisualizationFactory *factory, QWidget *parent): QWidget(parent) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMouseTracking(true);

    canvas_painter = new CanvasPainter(&rendered);
    rendered = QImage(width(), height(), QImage::Format_RGB32);
    
    connect(this, SIGNAL(paint_canvas(Canvas *)), canvas_painter, SLOT(paint_canvas(Canvas*)));
    connect(canvas_painter, SIGNAL(done()), SLOT(update()));
    
    formatter = factory->create_formatter();
}

Viewport::~Viewport() {

}

void Viewport::merge_canvas(Canvas* canvas) {
    local_canvas.combine_with(*canvas);
    emit paint_canvas(&local_canvas);
}

void Viewport::clear_canvas() {
    local_canvas.clear();
}

void Viewport::set_canvas_range(const DataRange &new_range) {
    local_canvas.set_range(new_range);
}

void Viewport::shift_range_to(const Timestamp &high_time) {
    DataRange range = local_canvas.get_range();
    qint64 time_difference = range.get_begin().get_time_element().ns_until(range.get_end().get_time_element());
    range.get_end().set_time_element(high_time);
    Timestamp timestamp = high_time;
    timestamp.add_ns(-time_difference);
    range.get_begin().set_time_element(timestamp);
    set_canvas_range(range);
}

void Viewport::force_repaint() {
    emit paint_canvas(&local_canvas);
}

void Viewport::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    CoordinateMapper mapper(size(), local_canvas.get_range());
    painter.drawImage(0, 0, rendered);
    
    QPen pen(Qt::DotLine);
    pen.setColor(qRgba(128, 128, 128, 64));
    
    painter.setPen(pen);
    
    qreal x_step = (width()-1) / 12.0;
    
    for(int x = 0; x <= 12; x ++) {
        painter.drawLine(x * x_step, 0, x * x_step, height()-1);
    }
    
    qreal y_step = (height()-1) / 12.0;
    
    for(int y = 0; y <= 12; y ++) {
        painter.drawLine(0, y * y_step, width()-1, y * y_step);
    }
    
    DataPoint point = mapper.map_to(QPointF(0, 0));
    static QFont grid_font = QFont("DejaVu Sans", 8);
    painter.setFont(grid_font);
    painter.drawText(20, 20, point.get_time_element().to_string());
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
    CoordinateMapper mapper(size(), local_canvas.get_range());
    DataPoint point = mapper.map_to(event->posF());
    emit mouse_position(formatter->format_point(point));
}

void Viewport::resizeEvent(QResizeEvent *event) {
    rendered = rendered.scaled(event->size());
    emit paint_canvas(&local_canvas);
}

void Viewport::wheelEvent(QWheelEvent *event) {
    DataRange range = local_canvas.get_range();
    CoordinateMapper mapper(size(), range);
    
    DataPoint mouse_position = mapper.map_to(event->pos());
    
    qint64 x_start = range.get_begin().get_time_element().to_ns();
    qint64 x_end = range.get_end().get_time_element().to_ns();
    qint64 x_range = x_end - x_start;
    qreal x_percentage = (mouse_position.get_time_element().to_ns() - x_start) / qreal(x_range);
    
    qreal y_start = range.get_begin().get_data_element();
    qreal y_end = range.get_end().get_data_element();
    qint64 y_range = y_end - y_start;
    qreal y_percentage = (mouse_position.get_data_element() - y_start) / qreal(y_range);
    
    qreal scale_amount = 1.0 - ((event->delta() / 120.0) / 10.0);
    
    qint64 new_x = x_start + (x_percentage*x_range / scale_amount) - (x_percentage*x_range);
    qint64 new_y = y_start + (y_percentage*y_range / scale_amount) - (y_percentage*y_range);
    
    range.get_begin().set_time_element(Timestamp(new_x));
    range.get_begin().set_data_element(new_y);
    range.get_end().set_time_element(Timestamp(new_x + x_range * scale_amount));
    range.get_end().set_data_element(new_y + y_range * scale_amount);
    
    set_canvas_range(range);
    emit paint_canvas(&local_canvas);
}
