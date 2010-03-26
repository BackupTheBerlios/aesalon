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
    
    @file session/VisualizationRenderPoint.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDER_POINT_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDER_POINT_H

#include "storage/Timestamp.h"

class VisualizationPoint {
private:
    Timestamp time_element;
    qint64 data_element;
public:
    VisualizationPoint(const Timestamp &time, qint64 data) : time_element(time), data_element(data) {}
    
    const Timestamp &get_time_element() const { return time_element; }
    void set_time_element(const Timestamp &new_time) { time_element = new_time; }
    
    qint64 get_data_element() const { return data_element; }
    void set_data_element(qint64 new_data) { data_element = new_data; }
};

#endif
