#ifndef AESALON_GUI_SESSION_VISUALIZATION_H
#define AESALON_GUI_SESSION_VISUALIZATION_H

#include <QWidget>
#include <QImage>
#include <QFormLayout>
#include <QScrollArea>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QCheckBox>

#include "VisualizationThread.h"
#include "VisualizationRequest.h"
#include "DataThread.h"
#include "TimeSlider.h"
#include "VisualizationDisplay.h"

class Visualization : public QWidget { Q_OBJECT
private:
    VisualizationThread *v_thread;
    DataThread *data_thread;
    VisualizationRequest *current_request;
    QVBoxLayout *main_layout;
    TimeSlider *from_slider;
    TimeSlider *to_slider;
    QCheckBox *follow_checkbox;
    VisualizationDisplay *display;
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
private slots:
    void update_slider_ranges();
    void handle_slider_change_from(Timestamp time);
    void handle_slider_change_to(Timestamp time);
signals:
    void visualization_request(VisualizationRequest *request);
};

#endif
