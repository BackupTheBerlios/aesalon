#include <QWhatsThis>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QFileDialog>

#include "Viewport.h"
#include "Viewport.moc"
#include "CanvasGenerator.h"

Viewport::Viewport(Canvas *canvas, VisualizationFactory *factory, QWidget *info_widget, QWidget *parent)
    : QWidget(parent), canvas(canvas), rendered_canvas(size(), DataRange()), info_widget(info_widget) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMouseTracking(true);
    setCursor(QCursor(Qt::CrossCursor));

    data_thread = factory->get_data_thread();

    canvas_painter = new CanvasPainter(data_thread);
    
    connect(this, SIGNAL(paint_canvas(QSize,Canvas*)), canvas_painter, SLOT(paint_canvas(QSize,Canvas*)), Qt::DirectConnection);
    connect(this, SIGNAL(paint_canvas(QSize,Canvas*,DataRange)), canvas_painter, SLOT(paint_canvas(QSize,Canvas*,DataRange)), Qt::DirectConnection);
    connect(canvas_painter, SIGNAL(done(RenderedCanvas)), SLOT(merge_canvas(RenderedCanvas)), Qt::DirectConnection);
    
    formatter = factory->create_formatter();
    click_handler = factory->create_click_handler(info_widget);
    
    update_timer = new QTimer();
    connect(update_timer, SIGNAL(timeout()), SLOT(update_timer_timeout()));
    update_timer->start(1000);
    
    click_lock = false;
    rt_attached = false;
}

Viewport::~Viewport() {

}

void Viewport::set_canvas_range(const DataRange &new_range) {
    rendered_canvas.set_range(new_range);
}

void Viewport::shift_range_to(const Timestamp &high_time) {
    Timestamp previous_high = rendered_canvas.get_range().get_end().get_time_element();
    qint64 diff = previous_high.ns_until(high_time);
    shift_range(DataPoint(-diff, 0.0));
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
    rendered_canvas.set_range(new_range);
    
    force_render();
}

void Viewport::save_screenshot() {
    QImage saved(size(), QImage::Format_RGB32);
    QPainter painter(&saved);
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
    
    pen.setColor(qRgba(255, 128, 0, 64));
    painter.setPen(pen);
    
    DataPoint point = mapper.map_to(QPointF(0, 0));
    static QFont grid_font = QFont("DejaVu Sans", 10, QFont::Bold);
    QFontMetrics metrics(grid_font);
    painter.setFont(grid_font);
    painter.drawText(5, metrics.height() + 5, formatter->format_point(point));
    point = mapper.map_to(QPointF(width()-1, height()-1));
    QString formatted = formatter->format_point(point);
    painter.drawText(width() - (metrics.width(formatted)+5), height()-6, formatted);
    
    QString label = "Time";
    painter.drawText((width()/2) - (metrics.width(label) / 2), height()-6, label);
    
    label = canvas->get_vertical_axis_label();
    painter.rotate(90.0);
    painter.drawText((height() / 2) - (metrics.width(label) / 2), -6, label);
    
    QString filename = QFileDialog::getSaveFileName(this, "Select file name", QString(), "PNG images (*.png)");
    
    /*qDebug("filename: \"%s\"", qPrintable(filename));*/
    
    if(filename.isEmpty()) return;
    if(!filename.endsWith(".png")) filename.append(".png");
    
    saved.save(filename);
}

void Viewport::toggle_attach(bool attached) {
    this->rt_attached = attached;
}

void Viewport::set_update_time(int new_time) {
    update_timer->setInterval(new_time);
}

void Viewport::merge_canvas(RenderedCanvas canvas) {
    rendered_canvas.merge(canvas);
    update();
}

void Viewport::repaint_regions() {
    QList<DataRange> ranges = canvas->take_updated_ranges();
    CoordinateMapper mapper(size(), rendered_canvas.get_range());
    foreach(DataRange range, ranges) {
        QRectF rect = mapper.map_to(range);
        request_paint(range);
    }
    update();
}

void Viewport::shift_range(const DataPoint &amount) {
    CoordinateMapper mapper(size(), rendered_canvas.get_range());
    QPointF offset = mapper.find_offset(amount);
    shift_range(offset);
}

void Viewport::shift_range(const QPointF &amount) {
    CoordinateMapper mapper(size(), rendered_canvas.get_range());
    DataRange local_range = rendered_canvas.get_range();
    DataPoint move_by = mapper.find_offset(amount);
    
    rendered_canvas.shift(move_by);
    rendered_canvas.shift(amount);
    
    DataRange exposed_range;
    if(move_by.get_data_element() > 0.0) {
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
}

void Viewport::update_timer_timeout() {
    repaint_regions();
    if(rt_attached) {
        shift_range_to(data_thread->get_last_time());
    }
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
    
    pen.setColor(qRgba(255, 128, 0, 64));
    painter.setPen(pen);
    
    DataPoint point = mapper.map_to(QPointF(0, 0));
    static QFont grid_font = QFont("DejaVu Sans", 10, QFont::Bold);
    QFontMetrics metrics(grid_font);
    painter.setFont(grid_font);
    painter.drawText(5, metrics.height() + 5, formatter->format_point(point));
    point = mapper.map_to(QPointF(width()-1, height()-1));
    QString formatted = formatter->format_point(point);
    painter.drawText(width() - (metrics.width(formatted)+5), height()-6, formatted);
    
    QString label = "Time";
    painter.drawText((width()/2) - (metrics.width(label) / 2), height()-6, label);
    
    label = canvas->get_vertical_axis_label();
    painter.rotate(90.0);
    painter.drawText((height() / 2) - (metrics.width(label) / 2), -6, label);
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
    CoordinateMapper mapper(size(), rendered_canvas.get_range());
    DataPoint point = mapper.map_to(event->posF());
    
    if(!click_lock) click_handler->handle_click(canvas, mapper.map_to(event->posF()));
    
    if(event->buttons() & Qt::LeftButton) {
        shift_range(event->posF() - old_mouse_pos);
        
        old_mouse_pos = event->posF();
    }
    emit mouse_position(formatter->format_point(point));
}

void Viewport::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) old_mouse_pos = event->posF();
    else if(event->button() == Qt::RightButton) {
        click_lock = !click_lock;
        emit lock_change(click_lock);
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

void Viewport::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
}

void Viewport::hideEvent(QHideEvent *event) {
    QWidget::hideEvent(event);
}
