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
    
    @file main/MainWindow.cpp
*/

#include <QApplication>
#include <QMetaType>

#include "MainWindow.h"
#include "MainWindow.moc"
#include "MainArea.h"
#include "Configuration.h"
#include "storage/Timestamp.h"

MainWindow::MainWindow(QWidget *parent) {
    setWindowTitle("Aesalon GUI");
    QSettings settings;
    setup_menus();
    setCentralWidget(new MainArea());
    /*this->showMaximized();*/
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(400, 200);
    setGeometry(0, 0, 800, 300);
}

void MainWindow::setup_menus() {
    aesalon_menu = new QMenu(tr("&Aesalon"));
    aesalon_menu->addAction("&Configuration . . .", this, SLOT(open_configuration()));
    aesalon_menu->addAction("&Quit", this, SLOT(close()));
    menuBar()->addMenu(aesalon_menu);
    
}

void MainWindow::open_configuration() {
    Configuration *config = new Configuration(this);
    config->exec();
}
