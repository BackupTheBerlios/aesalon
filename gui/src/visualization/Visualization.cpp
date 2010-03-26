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

Visualization::Visualization(VisualizationFactory *factory, DataThread *data_thread, QWidget *parent)
    : QWidget(parent), data_thread(data_thread) {
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    
    main_layout = new QVBoxLayout();
    
    position_label = new QLabel(tr("NYI"));
    QFont font = position_label->font();
    font.setBold(true);
    position_label->setFont(font);
    QHBoxLayout *status_layout = new QHBoxLayout();
    status_layout->addWidget(position_label);
    main_layout->addLayout(status_layout);
    
    display = factory->create_display(this);
    main_layout->addWidget(display);
    
    controller = new VisualizationController(data_thread, display->get_renderer(), this);
    controller->start();
    
    setLayout(main_layout);
}

Visualization::~Visualization() {
    controller->quit();
    controller->wait();
}
