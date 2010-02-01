#ifndef AESALON_ACTIVE_SESSION_BLOCK_VIEW_H
#define AESALON_ACTIVE_SESSION_BLOCK_VIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QSlider>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

#include "ActiveSessionMemory.h"

class ActiveSessionBlockView : public QWidget { Q_OBJECT
private:
    ActiveSessionMemorySnapshot *displayed_memory;
    QTableWidget *block_table;
    QVBoxLayout *main_layout;
    QHBoxLayout *slider_layout;
    QLabel *current_time;
    QSlider *time_slider;
    QCheckBox *realtime_checkbox;
    QDateTime start_time;
    QDateTime finish_time;
    QTimer *update_timer;
public:
    ActiveSessionBlockView(QWidget* parent = 0);
    virtual ~ActiveSessionBlockView() {}
private slots:
    void realtime_checkbox_toggled(bool new_state);
    void recalc_slider();
    void slider_value_change(int new_value);
    void slider_released();
public slots:
    void update_content(ActiveSessionMemorySnapshot *memory);
    void started(QDateTime time);
    void finished(QDateTime time);
signals:
    void request_realtime(bool value);
    void request_time_data(QDateTime time);
};

#endif