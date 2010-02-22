#ifndef AESALON_GUI_SESSION_VISUALIZATION_H
#define AESALON_GUI_SESSION_VISUALIZATION_H

#include <QWidget>
#include <QImage>
#include <QVBoxLayout>

#include "VisualizationThread.h"
#include "VisualizationRequest.h"
#include "DataThread.h"
#include "TimeSlider.h"

class Visualization : public QWidget { Q_OBJECT
private:
    VisualizationThread *v_thread;
    DataThread *data_thread;
    QImage *current_image;
    VisualizationRequest *current_request;
    QVBoxLayout *main_layout;
    TimeSlider *from_slider;
    TimeSlider *to_slider;
protected:
    virtual VisualizationThread *create_v_thread(DataThread *data_thread)
        { data_thread = data_thread; return NULL; }
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
protected:
    virtual void paintEvent(QPaintEvent *event);
private slots:
    void update_image(QImage *image);
signals:
    void visualization_request(VisualizationRequest *request);
};

#endif
