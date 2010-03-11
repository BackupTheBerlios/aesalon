/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file session/TimeSlider.h
*/

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
