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
    
    @file session/VisualizationDisplay.h
*/

#ifndef AESALON_GUI_SESSION_VISUALIZATION_DISPLAY_H
#define AESALON_GUI_SESSION_VISUALIZATION_DISPLAY_H

#include <QWidget>
#include <QLabel>
#include <QWheelEvent>

#include "VisualizationRenderer.h"

class VisualizationDisplay : public QWidget { Q_OBJECT
private:
    QPoint mouse_position;
    QImage canvas;
    VisualizationRenderer *renderer;
public:
    VisualizationDisplay(QWidget *parent);
    virtual ~VisualizationDisplay();
    
    VisualizationRenderer *get_renderer() const { return renderer; }
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
};

#endif
