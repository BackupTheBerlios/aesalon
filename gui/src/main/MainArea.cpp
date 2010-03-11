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
    
    @file main/MainArea.cpp
*/

#include <iostream>
#include "MainArea.h"
#include "MainArea.moc"

#include "DataSourceManager.h"
#include "data/NetworkSource.h"

MainArea::MainArea(QWidget *parent) {
    this->setTabsClosable(true);
    
    DataSourceManager *manager = new DataSourceManager(this);
    connect(manager, SIGNAL(tab_request(QWidget*,QString)), SLOT(add_tab(QWidget*,QString)));
    this->add_tab(manager, "&Data source manager");
    
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(tab_close_requested(int)));
}

void MainArea::add_tab(QWidget *tab, QString name) {
    this->addTab(tab, name);
    this->setCurrentWidget(tab);
}

void MainArea::remove_tab(QWidget *tab) {
    if(this->indexOf(tab)) this->removeTab(indexOf(tab));
}

void MainArea::tab_close_requested(int index) {
    if(index == 0) return;
    this->widget(index)->deleteLater();
}
