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
    
    @file session/VisualizationData.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_DATA_H
#define AESALON_GUI_SESSION_VISUALIZATION_DATA_H

#include <QPixmap>

#include "VisualizationRenderDataRange.h"

class VisualizationRequest;
class VisualizationRenderer;

class VisualizationData {
private:
    VisualizationRenderDataRange data_range;
public:
    VisualizationData();
    virtual ~VisualizationData();
    
    virtual void paint_onto(VisualizationRenderer *renderer) const = 0;
    
    virtual bool is_cachable() const = 0;
    
    VisualizationRenderDataRange &get_data_range() { return data_range; }
};

#endif
