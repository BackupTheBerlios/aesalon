#ifndef AESALON_GUI_VISUALIZATION_H
#define AESALON_GUI_VISUALIZATION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QMenu>

#include "Viewport.h"
#include "Controller.h"
#include "Renderer.h"
#include "session/DataThread.h"

class Visualization : public QWidget { Q_OBJECT
private:
    QHBoxLayout *upper_layout;
    QLabel *position_label;
    QSpinBox *cycle_time;
    QPushButton *request_button;
    QMenu *request_menu;
    Viewport *viewport;
    Controller *controller;
public:
    Visualization(VisualizationFactory *factory);
    virtual ~Visualization();
private slots:
    void set_position(QString formatted);
    
    void send_begin_rt();
    void send_end_rt();
    void send_render_full();
signals:
    void cycle_time_changed(int new_delay);
    void begin_rt();
    void end_rt();
    void render_full();
};

#endif
