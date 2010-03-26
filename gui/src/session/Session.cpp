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
    
    @file session/Session.cpp
*/

#include "Session.h"
#include "Session.moc"
#include "SessionOverview.h"

Session::Session(QWidget *parent, DataSource *data_source) : QWidget(parent), data_source(data_source) {
    data_thread = new DataThread(this, data_source);
    data_thread->start();
    tab_widget = new QTabWidget();
    tab_widget->setTabPosition(QTabWidget::West);
    
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(tab_widget);
    setLayout(layout);
    
    SessionOverview *overview = new SessionOverview(data_thread);
    connect(data_thread, SIGNAL(data_started()), overview, SLOT(started()));
    connect(data_thread, SIGNAL(data_finished()), overview, SLOT(finished()));
    connect(overview, SIGNAL(new_visualization(Visualization*)), SLOT(new_visualization(Visualization*)));
    tab_widget->addTab(overview, tr("&Overview"));
}

Session::~Session() {
    data_thread->quit();
    data_thread->wait();
}

void Session::new_visualization(Visualization *visualization) {
    /*tab_widget->addTab(visualization, visualization->get_title());*/
}
