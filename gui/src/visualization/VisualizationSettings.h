#ifndef AESALON_GUI_VISUALIZATION_SETTINGS_H
#define AESALON_GUI_VISUALIZATION_SETTINGS_H

#include <QDialog>
#include <QTabWidget>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>

#include "VisualizationUpdater.h"

class VisualizationSettings : public QDialog { Q_OBJECT
private:
    QVBoxLayout *main_layout;
    QTabWidget *tab_widget;
    QDialogButtonBox *lower_buttons;
    QWidget *normal_panel;
    QFormLayout *normal_layout;
    QWidget *realtime_panel;
    QFormLayout *realtime_layout;
    QSpinBox *realtime_history_length;
    QSpinBox *realtime_cycle_time;
public:
    VisualizationSettings(QWidget *parent);
    virtual ~VisualizationSettings();
public slots:
    virtual void accept();
signals:
    void change_updater(VisualizationUpdater *updater);
};

#endif
