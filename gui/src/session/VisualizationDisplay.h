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

#include <QLabel>
#include <QGraphicsView>
#include <QWheelEvent>

#include "VisualizationCanvas.h"

class VisualizationDisplay : public QGraphicsView { Q_OBJECT
private:
    VisualizationCanvas *canvas;
    QPoint mouse_position;
public:
    VisualizationDisplay(QWidget *parent);
    virtual ~VisualizationDisplay();
    
    VisualizationCanvas *get_canvas() const { return canvas; };
public slots:
    void change_canvas(VisualizationCanvas *new_canvas);
protected:
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
signals:
    void position_changed(qint64 time, qreal value);
    void update_request();
};

#endif
