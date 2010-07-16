#include <QResizeEvent>
#include <QPaintEvent>

#include "VisualizationWidget.h"
#include "VisualizationWidget.moc"

VisualizationWidget::VisualizationWidget(Module *module) : QWidget(NULL), m_module(module) {
	m_visualization = new Visualization(size(), module->defaultDataRange());
	module->visualize(m_visualization);
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
	m_visualization->clear();
	m_module->visualize(m_visualization);
	update();
}
