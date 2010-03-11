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
    
    @file session/VisualizationList.cpp
*/

#include "VisualizationList.h"
#include "VisualizationList.moc"
#include "visualizations/ActiveBlocksVisualization.h"
#include "visualizations/DensityVisualization.h"

VisualizationList::VisualizationList() {
    sortItems(Qt::AscendingOrder);
    addItem(ActiveBlocksVisualization::get_static_title());
    addItem(DensityVisualization::get_static_title());
}

VisualizationList::~VisualizationList() {

}

Visualization* VisualizationList::create_from_selected(DataThread *data_thread) const {
    if(currentItem() == NULL) return NULL;
    QString title = currentItem()->text();
    /* NOTE: Find a better way! */
    if(title == ActiveBlocksVisualization::get_static_title()) return new ActiveBlocksVisualization(data_thread);
    else if(title == DensityVisualization::get_static_title()) return new DensityVisualization(data_thread);
    else return NULL;
}
