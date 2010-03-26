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
    
    @file session/VisualizationDataRange.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDER_DATA_RANGE_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDER_DATA_RANGE_H

#include <QtGlobal>
#include <QRect>

#include "storage/Timestamp.h"

class VisualizationDataRange {
private:
    Timestamp lower_time, upper_time;
    qint64 lower_data, upper_data;
public:
    VisualizationDataRange();
    VisualizationDataRange(const Timestamp &lower_time, const Timestamp &upper_time, qint64 lower_data, qint64 upper_data);
    
    const Timestamp &get_lower_time() const { return lower_time; }
    void set_lower_time(const Timestamp &new_time) { lower_time = new_time; }
    const Timestamp &get_upper_time() const { return upper_time; }
    void set_upper_time(const Timestamp &new_time) { upper_time = new_time; }
    qint64 get_lower_data() const { return lower_data; }
    void set_lower_data(qint64 new_data) { lower_data = new_data; }
    qint64 get_upper_data() const { return upper_data; }
    void set_upper_data(qint64 new_data) { upper_data = new_data; }
};

#endif
