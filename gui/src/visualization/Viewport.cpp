#include <QWhatsThis>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

#include "Viewport.h"
#include "Viewport.moc"
#include "CanvasGenerator.h"

Viewport::Viewport(Canvas *canvas, VisualizationFactory *factory, QWidget *parent)
    : QWidget(parent), canvas(canvas), rendered_canvas(size(), DataRange()) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMouseTracking(true);
    setCursor(QCursor(Qt::CrossCursor));

    canvas_painter = new CanvasPainter();
    
    connect(this, SIGNAL(paint_canvas(QSize,Canvas*)), canvas_painter, SLOT(paint_canvas(QSize,Canvas*)), Qt::DirectConnection);
    connect(this, SIGNAL(paint_canvas(QSize,Canvas*,DataRange)), canvas_painter, SLOT(paint_canvas(QSize,Canvas*,DataRange)), Qt::DirectConnection);
    connect(canvas_painter, SIGNAL(done(RenderedCanvas)), SLOT(merge_canvas(RenderedCanvas)), Qt::DirectConnection);
    
    formatter = factory->create_formatter();
    click_handler = factory->create_click_handler();
    
    data_thread = factory->get_data_thread();
    
    update_timer = new QTimer();
    connect(update_timer, SIGNAL(timeout()), SLOT(repaint_regions()));
    update_timer->start(500);
}

Viewport::~Viewport() {

}

void Viewport::set_canvas_range(const DataRange &new_range) {
    rendered_canvas.set_range(new_range);
}

void Viewport::shift_range_to(const Timestamp &high_time) {
    qDebug("NYI: shift_range_to()");
    /*DataRange range = local_canvas.get_range();
    qint64 time_difference = range.get_begin().get_time_element().ns_until(range.get_end().get_time_element());
    range.get_end().set_time_element(high_time);
    Timestamp timestamp = high_time;
    timestamp.add_ns(-time_difference);
    range.get_begin().set_time_element(timestamp);
    set_canvas_range(range);*/
}

void Viewport::force_render() {
    emit paint_canvas(size(), canvas, rendered_canvas.get_range());
}

void Viewport::set_full_view() {
    if(!data_thread->get_start_time()) return;
    DataRange new_range = canvas->calculate_data_range();
    qint64 end_time;
    if(data_thread->get_finish_time()) end_time = data_thread->get_start_time()->ns_until(*data_thread->get_finish_time());
    else end_time = (Timestamp() - *data_thread->get_start_time()).to_ns();
    new_range.get_end().set_time_element(end_time);
    /*qDebug("end_time: %lli", end_time);*/
    rendered_canvas.set_range(new_range);
    
    force_render();
    
    /*qDebug("The new range is from (%s, %f) to (%s, %f).",
        qPrintable(rendered_canvas.get_range().get_begin().get_time_element().to_string()),
        rendered_canvas.get_range().get_begin().get_data_element(),
        qPrintable(rendered_canvas.get_range().get_end().get_time_element().to_string()),
        rendered_canvas.get_range().get_end().get_data_element());*/
}

void Viewport::merge_canvas(RenderedCanvas canvas) {
    rendered_canvas.merge(canvas);
    update();
}

void Viewport::repaint_regions() {
    /*qDebug("repaint_regions called . . .");*/
    QList<DataRange> ranges = canvas->take_updated_ranges();
    CoordinateMapper mapper(size(), rendered_canvas.get_range());
    foreach(DataRange range, ranges) {
        QRectF rect = mapper.map_to(range);
        /*qDebug("Asking to update rect: (%f, %f) to (%f, %f)", rect.left(), rect.top(), rect.right(), rect.bottom());*/
        request_paint(range);
    }
    update();
}

void Viewport::request_paint(DataRange range) {
    CoordinateMapper mapper(size(), rendered_canvas.get_range());
    
    emit paint_canvas(mapper.map_to(range).size().toSize(), canvas, range);
}

void Viewport::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    CoordinateMapper mapper(size(), rendered_canvas.get_range());
    if(!rendered_canvas.get_image().isNull()) painter.drawImage(0, 0, rendered_canvas.get_image());
    
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
    QFontMetrics metrics(grid_font);
    painter.setFont(grid_font);
    painter.drawText(5, metrics.height() + 5, formatter->format_point(point));
    point = mapper.map_to(QPointF(width()-1, height()-1));
    QString formatted = formatter->format_point(point);
    painter.drawText(width() - (metrics.width(formatted)+5), height()-6, formatted);
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
    CoordinateMapper mapper(size(), rendered_canvas.get_range());
    DataPoint point = mapper.map_to(event->posF());
    
    if(event->buttons() & Qt::LeftButton) {
        /*old_mouse_pos.setX(event->posF().x());*/
        /*old_mouse_pos.setY(old_mouse_pos.y() - 2.0);*/
        
        DataRange local_range = rendered_canvas.get_range();
        QPointF movement_delta = event->posF() - old_mouse_pos;
        /*DataPoint old_point = mapper.map_to(old_mouse_pos);*/
        /*DataPoint move_by = DataPoint(point.get_time_element() - old_point.get_time_element(), point.get_data_element() - old_point.get_data_element());*/
        /*DataPoint move_by = mapper.map_to(movement_delta);
        move_by.set_time_element(move_by.get_time_element() - local_range.get_begin().get_time_element());
        move_by.set_data_element(move_by.get_data_element() - local_range.get_begin().get_data_element());*/
        DataPoint move_by = mapper.find_offset(movement_delta);
        
        rendered_canvas.shift(move_by);
        rendered_canvas.shift(movement_delta);
        
        /*qDebug("move_by: time is %lli, data is %f", move_by.get_time_element().to_ns(), move_by.get_data_element());*/
        DataRange exposed_range;
        /*qDebug("Checking for movement-exposed regions . . .");*/
        if(move_by.get_data_element() > 0.0) {
            /*qDebug("Shifting upwards . . .");*/
            exposed_range.get_begin().set_time_element(local_range.get_begin().get_time_element());
            exposed_range.get_end().set_time_element(local_range.get_end().get_time_element());
            
            exposed_range.get_begin().set_data_element(local_range.get_end().get_data_element() - move_by.get_data_element());
            exposed_range.get_end().set_data_element(local_range.get_end().get_data_element() + move_by.get_data_element());
            
            emit paint_canvas(mapper.map_to(exposed_range).size().toSize(), canvas, exposed_range);
        }
        else if(move_by.get_data_element() < 0.0) {
            exposed_range.get_begin().set_time_element(local_range.get_begin().get_time_element());
            exposed_range.get_end().set_time_element(local_range.get_end().get_time_element());
            
            exposed_range.get_begin().set_data_element(local_range.get_begin().get_data_element() + move_by.get_data_element());
            exposed_range.get_end().set_data_element(local_range.get_begin().get_data_element() - move_by.get_data_element());
            emit paint_canvas(mapper.map_to(exposed_range).size().toSize(), canvas, exposed_range);
        }
        
        if(move_by.get_time_element().to_ns() > 0) {
            exposed_range.get_begin().set_data_element(rendered_canvas.get_range().get_begin().get_data_element());
            exposed_range.get_end().set_data_element(rendered_canvas.get_range().get_end().get_data_element());
            
            exposed_range.get_begin().set_time_element(local_range.get_end().get_time_element() - move_by.get_time_element());
            exposed_range.get_end().set_time_element(local_range.get_end().get_time_element() + move_by.get_time_element());
            
            emit paint_canvas(mapper.map_to(exposed_range).size().toSize(), canvas, exposed_range);
        }
        else if(move_by.get_time_element().to_ns() < 0) {
            exposed_range.get_begin().set_data_element(rendered_canvas.get_range().get_begin().get_data_element());
            exposed_range.get_end().set_data_element(rendered_canvas.get_range().get_end().get_data_element());
            
            exposed_range.get_begin().set_time_element(local_range.get_begin().get_time_element() + move_by.get_time_element());
            exposed_range.get_end().set_time_element(local_range.get_begin().get_time_element() - move_by.get_time_element());
            
            emit paint_canvas(mapper.map_to(exposed_range).size().toSize(), canvas, exposed_range);
        }
        
        old_mouse_pos = event->posF();
    }
    emit mouse_position(formatter->format_point(point));
}

void Viewport::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) old_mouse_pos = event->posF();
    else if(event->button() == Qt::RightButton) {
        CoordinateMapper mapper(size(), rendered_canvas.get_range());
        click_handler->handle_click(canvas, mapper.map_to(event->posF()));
    }
}

void Viewport::resizeEvent(QResizeEvent *event) {
    rendered_canvas.resize(event->size());
    force_render();
}

void Viewport::wheelEvent(QWheelEvent *event) {
    DataRange range = rendered_canvas.get_range();
    CoordinateMapper mapper(size(), range);
    
    DataPoint mouse_position = mapper.map_to(event->pos());
    
    qint64 x_start = range.get_begin().get_time_element().to_ns();
    qint64 x_end = range.get_end().get_time_element().to_ns();
    qint64 x_range = x_end - x_start;
    
    qreal y_start = range.get_begin().get_data_element();
    qreal y_end = range.get_end().get_data_element();
    qreal y_range = y_end - y_start;
    
    qreal scale_amount = 1.0 - ((event->delta() / 120.0) / 10.0);
    if(scale_amount < 0.1) scale_amount = 0.1;
    else if(scale_amount > 10.0) scale_amount = 10.0;

#if 0
    qreal x_percentage = (mouse_position.get_time_element().to_ns() - x_start) / qreal(x_range);
    qreal y_percentage = (mouse_position.get_data_element() - y_start) / qreal(y_range);

    qint64 new_x;
    qint64 new_x_range;
    qreal new_y;
    qreal new_y_range;
    
    if(event->modifiers() & Qt::ControlModifier) {
        new_x = x_start + (x_percentage*x_range / scale_amount) - (x_percentage*x_range);
        new_x_range = x_range * scale_amount;
        new_y = y_start;
        new_y_range = y_range;
    }
    else if(event->modifiers() & Qt::ShiftModifier) {
        new_x = x_start;
        new_x_range = x_range;
        new_y = y_start + (y_percentage*y_range / scale_amount) - (y_percentage*y_range);
        new_y_range = y_range * scale_amount;
    }
    else {
        new_x = x_start + (x_percentage*x_range / scale_amount) - (x_percentage*x_range);
        new_x_range = x_range * scale_amount;
        new_y = y_start + (y_percentage*y_range / scale_amount) - (y_percentage*y_range);
        new_y_range = y_range * scale_amount;
    }
    
    range.get_begin().set_time_element(Timestamp(new_x));
    range.get_begin().set_data_element(new_y);
    range.get_end().set_time_element(Timestamp(new_x + new_x_range));
    range.get_end().set_data_element(new_y + new_y_range);
#endif
    qint64 x_change;
    qreal y_change;
    
    if(event->modifiers() & Qt::ControlModifier) {
        x_change = x_range - (x_range * scale_amount);
        y_change = 0.0;
    }
    else if(event->modifiers() & Qt::ShiftModifier) {
        x_change = 0.0;
        y_change = y_range - (y_range * scale_amount);
    }
    else {
        x_change = x_range - (x_range * scale_amount);
        y_change = y_range - (y_range * scale_amount);
    }
    
    range.get_begin().set_time_element(x_start + (x_change / 2));
    range.get_end().set_time_element(x_start + (x_range - (x_change / 2)));
    range.get_begin().set_data_element(y_start + (y_change / 2));
    range.get_end().set_data_element(y_start + (y_range - (y_change / 2)));
    
    set_canvas_range(range);
    force_render();
}
