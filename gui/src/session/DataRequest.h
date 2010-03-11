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
    
    @file session/DataRequest.h
*/

#ifndef AESALON_GUI_SESSION_DATA_REQUEST_H
#define AESALON_GUI_SESSION_DATA_REQUEST_H

#include <QList>

class DataThread;
class VisualizationData;
class VisualizationThread;

class DataRequest {
private:
    VisualizationThread *v_thread;
public:
    DataRequest(VisualizationThread *v_thread) : v_thread(v_thread) {}
    virtual ~DataRequest() {}
    
    virtual void gather_data(DataThread *data_thread) = 0;
    virtual QList<VisualizationData *> create_data() = 0;
    
    VisualizationThread *get_v_thread() const { return v_thread; }
};

#endif
