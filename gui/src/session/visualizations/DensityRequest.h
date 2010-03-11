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
    
    @file session/visualizations/DensityRequest.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_DENSITY_REQUEST_H
#define AESALON_GUI_SESSION_VISUALIZATION_DENSITY_REQUEST_H

#include <QMap>

#include "../DataRequest.h"
#include "storage/Timestamp.h"
#include "storage/MemoryTypes.h"
#include "storage/EventVisitor.h"
#include "storage/Block.h"
#include "storage/BiTreeNode.h"
#include "DensityData.h"

class DensityRequest : public DataRequest, public EventVisitor {
private:
    Timestamp from, to;
    QList<VisualizationData *> data_list;
    
    QMap<MemoryAddress, DensityData *> density_data;
    static QList<QRgb> colours;
    int colour_index;
    QRgb get_colour();
public:
    DensityRequest(VisualizationThread* v_thread, const Timestamp &from, const Timestamp &to);
    virtual ~DensityRequest();
    virtual QList<VisualizationData*> create_data();
    virtual void gather_data(DataThread* data_thread);
    
    virtual void visit(AllocEvent* event);
    virtual void visit(FreeEvent* event);
private:
    void gather_blocks(BiTreeNode *node = NULL);
};

#endif
