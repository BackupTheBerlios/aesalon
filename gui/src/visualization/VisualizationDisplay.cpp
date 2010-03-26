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
    
    @file session/VisualizationDisplay.cpp
*/

#include <QPainter>
#include "VisualizationDisplay.h"
#include "VisualizationDisplay.moc"

VisualizationDisplay::VisualizationDisplay(const VisualizationFactory *factory, QWidget *parent): QWidget(parent), renderer(renderer) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setCursor(Qt::CrossCursor);
    setBackgroundRole(QPalette::BrightText);
    
    canvas = QImage(size(), QImage::Format_ARGB32);
    canvas.fill(qRgba(255, 255, 255, 255));
    
    renderer = factory->create_renderer(&canvas);
}

VisualizationDisplay::~VisualizationDisplay() {

}

void VisualizationDisplay::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawImage(0, 0, canvas);    
    
    QPen pen(Qt::DotLine);
    pen.setColor(qRgb(192, 192, 192));
    painter.setPen(pen);
    qreal x_step = (width()-1) / 12;
    for(int x = 0; x <= 12; x ++) {
        painter.drawLine(x_step * x, 0, x_step * x, height()-1);
    }
    
    qreal y_step = (height()-1) / 12.0;
    for(int y = 0; y <= 12; y ++) {
        painter.drawLine(0, y_step * y, width()-1, y_step * y);
    }

    
    QWidget::paintEvent(event);
}

void VisualizationDisplay::wheelEvent(QWheelEvent *event) {
    QWidget::wheelEvent(event);
}

void VisualizationDisplay::mouseMoveEvent(QMouseEvent* event) {
    QWidget::mouseMoveEvent(event);
}

void VisualizationDisplay::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton)
        mouse_position = event->globalPos();
    QWidget::mousePressEvent(event);
}

void VisualizationDisplay::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
}

void VisualizationDisplay::keyReleaseEvent(QKeyEvent *event) {
    QWidget::keyReleaseEvent(event);
}

void VisualizationDisplay::resizeEvent(QResizeEvent* event) {
    canvas = canvas.scaled(event->size());
    update();
}
