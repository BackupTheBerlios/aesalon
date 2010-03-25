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
    
    @file session/VisualizationRenderer.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_RENDERER_H
#define AESALON_GUI_SESSION_VISUALIZATION_RENDERER_H

#include <QPixmap>
#include <QWidget>

#include "VisualizationRenderDataRange.h"
#include "VisualizationRenderPoint.h"
#include "VisualizationData.h"

class VisualizationRenderer {
private:
    QList<VisualizationRenderDataRange> gaps;
    VisualizationRenderDataRange range;
    QList<VisualizationData *> data_list;
    bool can_split;
    
    VisualizationRenderPoint graph_point;
    bool graph_point_valid;
    
    void recalc_ranges();
    void paint_grid();
    void render_data();
    QPointF resolve_point(const VisualizationRenderPoint &point) const;
public:
    VisualizationRenderer(bool can_split);
    virtual ~VisualizationRenderer();
    
    void update();
    void add_data(VisualizationData *data);
    
    /* NOTE: this *need* to be reentrant! */
    void paint_line(const VisualizationRenderPoint &from, const VisualizationRenderPoint &to,
        QRgb colour, Qt::PenStyle style = Qt::SolidLine);
    void paint_box(const VisualizationRenderPoint &from, const VisualizationRenderPoint &to,
        QRgb line_colour, QRgb fill_colour, Qt::PenStyle line_style = Qt::SolidLine, Qt::BrushStyle fill_style = Qt::SolidPattern);
    void paint_text(const VisualizationRenderPoint &point, QString text, int size, QRgb colour);
    void paint_graph_element(const VisualizationRenderPoint &point, QRgb colour);
};

#endif
