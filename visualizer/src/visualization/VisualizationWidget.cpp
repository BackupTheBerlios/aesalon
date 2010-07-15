#include <QResizeEvent>
#include <QPaintEvent>

#include "VisualizationWidget.h"
#include "VisualizationWidget.moc"

VisualizationWidget::VisualizationWidget(DataRange range) : QWidget(NULL) {
	m_visualization = new Visualization(size(), range);
}

VisualizationWidget::~VisualizationWidget() {

}

void VisualizationWidget::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	
	painter.drawPixmap(0, 0, width(), height(), m_visualization->pixmap());
	
}

void VisualizationWidget::resizeEvent(QResizeEvent *event) {
	m_visualization->resize(event->size());
	QWidget::resizeEvent(event);
}
