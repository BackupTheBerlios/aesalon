#ifndef AESALON_ACTIVE_SESSION_BLOCK_VIEW_H
#define AESALON_ACTIVE_SESSION_BLOCK_VIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QSlider>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

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
public:
    ActiveSessionBlockView(QWidget* parent = 0);
    virtual ~ActiveSessionBlockView() {}
private:
    void realtime_checkbox_toggled(bool new_state);
public slots:
    void update_content(ActiveSessionMemorySnapshot *memory);
signals:
    void request_realtime(bool value);
};

#endif