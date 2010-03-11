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
    
    @file session/DataRequestQueue.h
*/

#ifndef AESALON_GUI_SESSION_DATA_REQUEST_QUEUE_H
#define AESALON_GUI_SESSION_DATA_REQUEST_QUEUE_H

#include <QQueue>
#include <QMutex>
#include "DataRequest.h"

class DataRequestQueue {
private:
    static const int request_list_size = 1000;
    QQueue<DataRequest *> request_list;
    QMutex list_mutex;
    int last_slot;
public:
    DataRequestQueue();
    virtual ~DataRequestQueue();
    
    void push_request(DataRequest *request);
    DataRequest *pop_request();
    int current_requests();
    void clear_queue();
};

#endif
