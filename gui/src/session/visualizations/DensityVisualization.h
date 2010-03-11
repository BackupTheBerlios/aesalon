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
    
    @file session/visualizations/DensityVisualization.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_DENSITY_H
#define AESALON_GUI_SESSION_VISUALIZATION_DENSITY_H

#include "../Visualization.h"

class DensityVisualization : public Visualization {
public:
    DensityVisualization(DataThread* data_thread, QWidget* parent = 0);
    virtual ~DensityVisualization();
    
    static QString get_static_title() {
        return "Block Density";
    }
    virtual QString get_title() const {
        return "Block Density";
    }
protected:
    virtual VisualizationThread* create_v_thread(DataThread *data_thread);
};

#endif
