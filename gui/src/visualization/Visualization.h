#ifndef AESALON_GUI_VISUALIZATION_H
#define AESALON_GUI_VISUALIZATION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QMenu>
#include <QCheckBox>

#include "Viewport.h"
#include "Renderer.h"
#include "session/DataThread.h"
#include "FilterManager.h"

class Visualization : public QWidget { Q_OBJECT
private:
    QHBoxLayout *upper_layout;
    QLabel *position_label;
    QSpinBox *cycle_time;
    QPushButton *request_button;
    QMenu *request_menu;
    QWidget *info_widget;
    Viewport *viewport;
    Renderer *renderer;
    Canvas *canvas;
    QCheckBox *lock_box;
    FilterManager *filter_manager;
public:
    Visualization(VisualizationFactory *factory);
    virtual ~Visualization();
private slots:
    void set_position(QString formatted);
signals:
    void cycle_time_changed(int new_delay);
};

#endif
