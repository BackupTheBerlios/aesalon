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
    
    @file session/visualizations/DensityThread.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_DENSITY_THREAD_H
#define AESALON_GUI_SESSION_VISUALIZATION_DENSITY_THREAD_H

#include "../VisualizationThread.h"

class DensityThread : public VisualizationThread {
protected:
    /*virtual void generate_requests(VisualizationRequest* current_request);*/
    virtual bool is_splittable() const { return false; }
public:
    DensityThread(DataThread* data_thread, QObject* parent = 0);
    virtual ~DensityThread();
};

#endif
