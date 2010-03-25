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
    
    @file session/Visualization.cpp
*/

#include <QPainter>
#include <QLabel>
#include <QScrollBar>
#include <QEvent>
#include <QWheelEvent>
#include "Visualization.h"
#include "Visualization.moc"

Visualization::Visualization(DataThread *data_thread, QWidget *parent)
    : QWidget(parent), v_thread(NULL), data_thread(data_thread) {
    
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    
    main_layout = new QVBoxLayout();
    
    from_slider = new TimeSlider(tr("From:"));
    connect(from_slider, SIGNAL(changed(Timestamp)), SLOT(handle_slider_change_from(Timestamp)));
    main_layout->addWidget(from_slider);
    to_slider = new TimeSlider(tr("To:"));
    connect(to_slider, SIGNAL(changed(Timestamp)), SLOT(handle_slider_change_to(Timestamp)));
    main_layout->addWidget(to_slider);
    
    follow_checkbox = new QCheckBox(tr("&Real-time updating"));
    follow_checkbox->setCheckState(Qt::Unchecked);
    position_label = new QLabel();
    QFont font = position_label->font();
    font.setBold(true);
    position_label->setFont(font);
    QHBoxLayout *status_layout = new QHBoxLayout();
    status_layout->addWidget(follow_checkbox);
    status_layout->addWidget(position_label);
    main_layout->addLayout(status_layout);
    
    display = new VisualizationDisplay(this);
    connect(display, SIGNAL(position_changed(qint64,qreal)), SLOT(position_changed(qint64,qreal)));
    main_layout->addWidget(display);
    
    setLayout(main_layout);
    
    QTimer *ranges_time = new QTimer(this);
    connect(ranges_time, SIGNAL(timeout()), SLOT(update_slider_ranges()));
    ranges_time->start(500);
}

Visualization::~Visualization() {
    if(v_thread) {
        v_thread->get_request_queue()->push_request(NULL);
        v_thread->quit();
        v_thread->wait();
    }
}

void Visualization::initialize() {
    v_thread = create_v_thread(data_thread);
    if(v_thread == NULL) {
        qDebug("Failed to create v_thread!");
        this->deleteLater();
        return;
    }
    connect(this, SIGNAL(visualization_request(VisualizationRequest*)), v_thread, SLOT(update_request(VisualizationRequest*)));
    connect(display, SIGNAL(update_request()), v_thread, SLOT(render_request()));
    v_thread->start();
    if(data_thread->get_start_time()) {
        from_slider->set_range(*data_thread->get_start_time(), Timestamp());
    }
}

void Visualization::update_slider_ranges() {
    if(data_thread->get_start_time()) {
        if(data_thread->get_finish_time()) {
            from_slider->set_range(*data_thread->get_start_time(), *data_thread->get_finish_time());
            to_slider->set_range(*data_thread->get_start_time(), *data_thread->get_finish_time());
        }
        else {
            from_slider->set_range(*data_thread->get_start_time(), Timestamp());
            to_slider->set_range(*data_thread->get_start_time(), Timestamp());
        }
    }
}

void Visualization::handle_slider_change_from(Timestamp time) {
    if(time > to_slider->current_value()) {
        time.add_ms(1);
        to_slider->set_value(time);
    }
    /* NOTE: deleting this will crash the program if data is being visualized! */
    current_request = new VisualizationRequest(from_slider->current_value(), to_slider->current_value());
    emit visualization_request(current_request);
}

void Visualization::handle_slider_change_to(Timestamp time) {
    if(time < from_slider->current_value()) {
        time = from_slider->current_value();
        time.add_ms(20);
        to_slider->set_value(time);
    }
    /* NOTE: deleting this will crash the program if data is being visualized! */
    current_request = new VisualizationRequest(from_slider->current_value(), to_slider->current_value());
    emit visualization_request(current_request);
}

void Visualization::position_changed(qint64 time, qreal value) {
    if(time < 0) time = 0;
    position_label->setText(QString().sprintf("%02lli:%02lli:%03lli,%lli", (time / 1000) / 60, (time / 1000) % 60, time % 1000, qint64(value)));
}
