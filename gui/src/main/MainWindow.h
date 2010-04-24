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
    
    @file main/MainWindow.h
*/

#ifndef AESALON_GUI_MAIN_WINDOW_H
#define AESALON_GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMenu>
#include <QMenuBar>

class MainWindow : public QMainWindow { Q_OBJECT
private:
    void setup_menus();
    
    QMenu *aesalon_menu;
    
    QMenu *help_menu;
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow() {}
private slots:
    void about();
public slots:
    void open_configuration();
};

#endif
