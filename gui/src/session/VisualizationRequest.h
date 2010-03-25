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
    
    @file session/VisualizationRequest.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_REQUEST_H
#define AESALON_GUI_SESSION_VISUALIZATION_REQUEST_H

#include "storage/Timestamp.h"
#include "DataRequest.h"
#include "VisualizationRenderer.h"

class VisualizationRequest {
private:
    Timestamp from, to;
    VisualizationRenderer *renderer;
public:
    VisualizationRequest(const Timestamp &from, const Timestamp &to);
    
    const Timestamp &get_from() const { return from; }
    const Timestamp &get_to() const { return to; }
    void set_renderer(VisualizationRenderer *new_renderer) { renderer = new_renderer; }
    VisualizationRenderer *get_renderer() const { return renderer; }
};

#endif
