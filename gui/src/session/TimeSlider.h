#ifndef AESALON_GUI_SESSION_TIME_SLIDER_H
#define AESALON_GUI_SESSION_TIME_SLIDER_H

#include <QWidget>
#include <QSlider>
#include <QHBoxLayout>
#include <QLineEdit>

#include "storage/Timestamp.h"

class TimeSlider : public QWidget { Q_OBJECT
private:
    QLineEdit *display;
    QSlider *slider;
    Timestamp from;
    QString prefix;
public:
    TimeSlider(QString prefix, QWidget *parent = NULL);
    virtual ~TimeSlider();
    void set_range(const Timestamp &from, const Timestamp &to);
    
    Timestamp current_value() const;
public slots:
    void set_value(const Timestamp &time);
private slots:
    void update_display(int new_value);
    void update_slider();
    void emit_changed();
signals:
    void changed(Timestamp new_value);
};

#endif
