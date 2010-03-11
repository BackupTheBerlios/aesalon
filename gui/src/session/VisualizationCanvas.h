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
    
    @file session/VisualizationCanvas.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_CANVAS_H
#define AESALON_GUI_SESSION_VISUALIZATION_CANVAS_H

#include <QGraphicsScene>
#include "VisualizationRenderDataRange.h"

class VisualizationCanvas : public QGraphicsScene { Q_OBJECT
private:
    VisualizationRenderDataRange data_range;
public:
    VisualizationCanvas(QObject* parent = 0);
    virtual ~VisualizationCanvas();
    
    const VisualizationRenderDataRange &get_data_range() { return data_range; }
    void set_data_range(const VisualizationRenderDataRange &new_range) { data_range = new_range; }
};

#endif
