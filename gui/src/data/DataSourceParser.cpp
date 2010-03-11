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
    
    @file data/DataSourceParser.cpp
*/

#include "session/DataThread.h"
#include "DataSourceParser.h"
#include "NetworkSource.h"

DataSource *DataSourceParser::parse_data_source(QSettings *settings) {
    QString name = settings->value("name").toString();
    QString type = settings->value("type").toString();
    if(type == "network") {
        QString host = settings->value("network-host").toString();
        quint16 port = settings->value("network-port").toUInt();
        return new NetworkSource(name, host, port);
    }
    else return NULL;
}
