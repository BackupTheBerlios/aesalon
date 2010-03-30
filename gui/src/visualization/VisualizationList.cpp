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

#include "DensityFactory.h"
#include "BlockCountFactory.h"

VisualizationList::VisualizationList() {
    sortItems(Qt::AscendingOrder);
    addItem(tr("Block Count"));
    addItem(tr("Density Visualization"));
}

VisualizationList::~VisualizationList() {

}

Visualization* VisualizationList::create_from_selected(DataThread *data_thread) const {
    if(currentItem() == NULL) return NULL;
    QString title = currentItem()->text();
    if(title == tr("Density Visualization")) {
        return new Visualization(new DensityFactory(), data_thread);
    }
    else if(title == tr("Block Count")) {
        return new Visualization(new BlockCountFactory(), data_thread);
    }
    return NULL;
}
