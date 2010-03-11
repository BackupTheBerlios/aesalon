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
    
    @file session/TimeSlider.cpp
*/

#include <cmath>
#include <QTextStream>
#include <QDebug>
#include "TimeSlider.h"
#include "TimeSlider.moc"

TimeSlider::TimeSlider(QString prefix, QWidget* parent) : QWidget(parent), prefix(prefix) {
    setLayout(new QHBoxLayout());
    display = new QLineEdit(prefix + " 00:00.000");
    display->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    connect(display, SIGNAL(returnPressed()), SLOT(update_slider()));
    layout()->addWidget(display);
    
    slider = new QSlider();
    slider->setOrientation(Qt::Horizontal);
    slider->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    slider->setInvertedControls(true);
    slider->setSingleStep(100);
    slider->setPageStep(5000);
    slider->setMaximum(0);
    connect(slider, SIGNAL(valueChanged(int)), SLOT(update_display(int)));
    connect(slider, SIGNAL(sliderReleased()), SLOT(emit_changed()));
    layout()->addWidget(slider);
}

TimeSlider::~TimeSlider() {

}

void TimeSlider::set_range(const Timestamp& from, const Timestamp& to) {
    slider->setMaximum(from.ms_until(to));
    this->from = from;
}

Timestamp TimeSlider::current_value() const {
    Timestamp time = from;
    time.add_ms(slider->value());
    return time;
}

void TimeSlider::set_value(const Timestamp& time) {
    slider->setValue(from.ms_until(time));
}

void TimeSlider::update_display(int new_value) {
    if(new_value < 0) return;
    QString string;
    string.sprintf("%s %02i:%02i.%03i", prefix.toLocal8Bit().constData(), (new_value / 1000) / 60, (new_value / 1000) % 60, new_value % 1000);
    display->setText(string);
}

void TimeSlider::update_slider() {
    QString string = display->text();
    QByteArray input = string.toAscii();
    int i = 0;
    QString minutes, seconds, milliseconds;
    string.remove(prefix);
    for(; i < string.size(); i ++) {
        if(isdigit(input[i])) {
            minutes += input[i];
            continue;
        }
        else if(input[i] == ':') break;
        return;
    }
    if(input[i] != ':') return;
    i ++;
    for(; i < string.size(); i ++) {
        if(isdigit(input[i])) {
            seconds += input[i];
            continue;
        }
        else if(input[i] == '.') break;
        return;
    }
    if(i != string.size()) {
        if(input[i] != '.') return;
        i ++;
        for(; i < string.size(); i ++) {
            if(isdigit(input[i])) {
                milliseconds += input[i];
                continue;
            }
            return;
        }
    }
    int value;
    QTextStream stream(&milliseconds);
    stream >> i;
    value = i;
    stream.setString(&seconds);
    stream >> i;
    value += i * 1000;
    stream.setString(&minutes);
    stream >> i;
    value += i * 60000;
    slider->setValue(value);
    emit_changed();
}

void TimeSlider::emit_changed() {
    Timestamp time = from;
    time.add_ms(slider->value());
    emit changed(time);
}
