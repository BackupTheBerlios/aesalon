#ifndef AESALON_GUI_SESSION_VISUALIZATION_H
#define AESALON_GUI_SESSION_VISUALIZATION_H

#include <QWidget>
#include <QImage>
#include <QFormLayout>
#include <QScrollArea>
#include <QLabel>
#include <QCheckBox>

#include "VisualizationThread.h"
#include "VisualizationRequest.h"
#include "DataThread.h"
#include "TimeSlider.h"

class VisualizationCanvas : public QScrollArea { Q_OBJECT
private:
    QPixmap *image;
    QTimer *update_timer;
    QLabel *image_label;
    qreal scale;
    QPoint last_mouse_position;
    QSize canvas_size;
private:
    void calc_canvas_size();
    QPointF resolve_point(const VisualizationRenderPoint &point) const;
public:
    VisualizationCanvas(QWidget *parent);
    virtual ~VisualizationCanvas() {}
    
    QPixmap *get_image() const { return image; }
    QSize *get_canvas_size() { return &canvas_size; }
protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
public slots:
    void update_image(QPixmap *image);
    void image_updated();
    void set_scale(qreal new_scale);
signals:
    void request_rerender();
};

class Visualization : public QWidget { Q_OBJECT
private:
    VisualizationThread *v_thread;
    DataThread *data_thread;
    VisualizationRequest *current_request;
    VisualizationCanvas *canvas;
    QVBoxLayout *main_layout;
    TimeSlider *from_slider;
    TimeSlider *to_slider;
    QCheckBox *follow_checkbox;
    QSize *canvas_size;
protected:
    virtual VisualizationThread *create_v_thread(DataThread *data_thread, QSize *canvas_size)
        { data_thread = data_thread; canvas_size = canvas_size; return NULL; }
public:
    Visualization(DataThread *data_thread, QWidget *parent = 0);
    virtual ~Visualization();
    
    virtual void initialize();
    
    static QString get_static_title() {
        return "ERROR!";
    }
    virtual QString get_title() const {
        return "ERROR!";
    }
private slots:
    void update_slider_ranges();
    void handle_slider_change_from(Timestamp time);
    void handle_slider_change_to(Timestamp time);
signals:
    void visualization_request(VisualizationRequest *request);
};

#endif
