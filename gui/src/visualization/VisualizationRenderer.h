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

#include <QObject>
#include <QColor>
#include <QImage>
#include <QPainter>

#include "VisualizationDataRange.h"
#include "VisualizationPoint.h"
#include "storage/EventVisitor.h"
#include "storage/Snapshot.h"

class VisualizationRenderer : public QObject, public EventVisitor { Q_OBJECT
private:
    QImage *image;
    QList<VisualizationDataRange> gaps;
    VisualizationDataRange range;
    bool can_split;
    
    VisualizationPoint graph_point;
    bool graph_point_valid;
    QPainter painter;
public:
    VisualizationRenderer(QImage *image, bool can_split);
    virtual ~VisualizationRenderer();

    VisualizationDataRange &get_range() { return range; }

    QPointF resolve_point(const VisualizationPoint &point) const;
    QRectF resolve_range(const VisualizationDataRange &rect) const;
    VisualizationPoint resolve_point(const QPointF &point) const;

    virtual void begin_update(Snapshot *starting_snaphot);
    virtual void end_update();
public slots:
    void update_range(const VisualizationDataRange &new_range);
protected:
    /* NOTE: these *need* to be reentrant! */
    void paint_line(const VisualizationPoint &from, const VisualizationPoint &to,
        QRgb colour, Qt::PenStyle style = Qt::SolidLine);
    void paint_box(const VisualizationPoint &from, const VisualizationPoint &to,
        QRgb line_colour, QRgb fill_colour, Qt::PenStyle line_style = Qt::SolidLine, Qt::BrushStyle fill_style = Qt::SolidPattern);
    void paint_box(const VisualizationDataRange &range, QRgb line_colour, QRgb fill_colour,
        Qt::PenStyle line_style = Qt::SolidLine, Qt::BrushStyle fill_style = Qt::SolidPattern);
    void paint_text(const VisualizationPoint &point, QString text, int size, QRgb colour);
    void paint_graph_element(const VisualizationPoint &point, QRgb colour);
};

#endif
