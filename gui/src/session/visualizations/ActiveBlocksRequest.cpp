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
    
    @file session/visualizations/ActiveBlocksRequest.cpp
*/

#include <QStack>
#include "ActiveBlocksRequest.h"
#include "ActiveBlocksData.h"
#include "../DataThread.h"
#include "storage/AllocEvent.h"
#include "storage/FreeEvent.h"

ActiveBlocksRequest::ActiveBlocksRequest(VisualizationThread* v_thread, const Timestamp &from, const Timestamp &to)
    : DataRequest(v_thread), from(from), to(to) {
}

ActiveBlocksRequest::~ActiveBlocksRequest() {
    
}

void ActiveBlocksRequest::visit(AllocEvent* event) {
    block_count ++;
    data_list.append(new ActiveBlocksData(event->get_timestamp(), block_count));
}

void ActiveBlocksRequest::visit(FreeEvent *event) {
    block_count --;
    data_list.append(new ActiveBlocksData(event->get_timestamp(), block_count));
}

QList<VisualizationData*> ActiveBlocksRequest::create_data() {
    return data_list;
}

void ActiveBlocksRequest::gather_data(DataThread* data_thread) {
    Snapshot *starting_snapshot = data_thread->get_snapshot_list()->get_snapshot_for(from); 
    block_count = starting_snapshot->get_block_count();
    data_list.append(new ActiveBlocksData(from, block_count));
    data_thread->get_snapshot_list()->iterate_through(from, to, *this);
}

