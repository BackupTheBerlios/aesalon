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
    
    @file data/DataSource.h
*/

#ifndef AESALON_GUI_DATA_SOURCE_H
#define AESALON_GUI_DATA_SOURCE_H

#include <QString>
#include <QSettings>

#include "DataReceiver.h"

class DataSource {
private:
    QString name;
public:
    DataSource(QString name) : name(name) {}
    virtual ~DataSource() {}
    
    const QString &get_name() const { return name; }
    void set_name(const QString &new_name) { name = new_name; }
    
    virtual DataReceiver *spawn_receiver(DataThread *data_thread) const = 0;
    
    virtual void save_to(QSettings *settings) = 0;
};

#endif
