#ifndef AESALON_GUI_VISUALIZATION_H
#define AESALON_GUI_VISUALIZATION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>

#include "Viewport.h"
#include "Controller.h"
#include "Renderer.h"
#include "session/DataThread.h"

class Visualization : public QWidget { Q_OBJECT
private:
    QHBoxLayout *upper_layout;
    QLabel *position_label;
    QSpinBox *cycle_time;
    QPushButton *attach_button;
    Viewport *viewport;
    Controller *controller;
public:
    Visualization(Renderer *renderer, DataThread *data_thread);
    virtual ~Visualization();
signals:
    void attach_requested();
    void cycle_time_changed(int new_delay);
};

#endif
