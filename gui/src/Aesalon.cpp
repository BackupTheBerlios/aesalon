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
    
    @file Aesalon.cpp
    @mainpage Aesalon GUI doxygen documentation
    Aesalon is a project that allows a user to visualize, in real-time, the
    dynamically-allocated memory of a program. The project is split in two
    primary sections: the monitor and the GUI. This is the documentation for
    the GUI.
    
    Aesalon was originally designed and written by strange &lt;kawk256@gmail.com&gt;.
*/

#include <QApplication>
#include "main/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QCoreApplication::setOrganizationName("aesalon");
    QCoreApplication::setApplicationName("gui");
    
    MainWindow mw;
    mw.show();
    return app.exec();
}