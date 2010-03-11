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
    
    @file session/visualizations/DensityData.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_DENSITY_DATA_H
#define AESALON_GUI_SESSION_VISUALIZATION_DENSITY_DATA_H

#include "../VisualizationData.h"
#include "storage/MemoryTypes.h"

class DensityData : public VisualizationData {
private:
    Timestamp allocated_at, released_at;
    MemoryAddress address;
    MemorySize size;
    bool complete;
    QRgb colour;
public:
    DensityData(const Timestamp &allocated_at, const Timestamp &released_at, MemoryAddress address, MemorySize size, bool complete);
    virtual ~DensityData();
    
    void set_release_time(const Timestamp &time);
    void set_complete() { complete = true; }
    void set_colour(QRgb new_colour) { colour = new_colour; }
    
    virtual bool is_cachable() const;
    virtual void paint_onto(VisualizationRenderer* renderer) const;
};

#endif
