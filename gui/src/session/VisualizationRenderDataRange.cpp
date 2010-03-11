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
    
    @file session/VisualizationRenderDataRange.cpp
*/

#include "VisualizationRenderDataRange.h"

VisualizationRenderDataRange::VisualizationRenderDataRange()
    : lower_time(Timestamp(0)), upper_time(Timestamp(0)),
    lower_data(0), upper_data(0) {
    
}


VisualizationRenderDataRange::VisualizationRenderDataRange(const Timestamp &lower_time, const Timestamp &upper_time, qint64 lower_data, qint64 upper_data)
    : lower_time(lower_time), upper_time(upper_time), lower_data(lower_data), upper_data(upper_data) {
    
}
