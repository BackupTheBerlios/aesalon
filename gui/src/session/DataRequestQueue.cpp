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
    
    @file session/DataRequestQueue.cpp
*/

#include "DataRequestQueue.h"

DataRequestQueue::DataRequestQueue() {
}

DataRequestQueue::~DataRequestQueue() {
    
}

void DataRequestQueue::push_request(DataRequest *request) {
    list_mutex.lock();
    request_list.enqueue(request);
    list_mutex.unlock();
}

DataRequest *DataRequestQueue::pop_request() {
    /* Do this out of order so that the mutex is locked for the smallest length of time possible. */
    /* NOTE: dubious speed gains are offset by an obvious complexity increase . . . */
    DataRequest *request = NULL;
    list_mutex.lock();
    request = request_list.dequeue();
    list_mutex.unlock();
    return request;
}

int DataRequestQueue::current_requests() {
    int count;
    list_mutex.lock();
    count = request_list.count();
    list_mutex.unlock();
    return count;
}
void DataRequestQueue::clear_queue() {
    list_mutex.lock();
    request_list.clear();
    list_mutex.unlock();
}
