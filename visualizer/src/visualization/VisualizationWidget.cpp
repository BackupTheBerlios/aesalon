#include <QResizeEvent>
#include <QPaintEvent>

#include "VisualizationWidget.h"
#include "VisualizationWidget.moc"

VisualizationWidget::VisualizationWidget(Module *module) : QWidget(NULL), m_module(module) {
	m_visualization = new Visualization(size(), module->defaultDataRange());
	m_controller = new VisualizationController(module, m_visualization);
	/*m_controller->fullVisualization();*/
	/* A resizeEvent will be generated once the widget has been added to a layout;
		when that happens, a full visualization will be produced. */
}

VisualizationWidget::~VisualizationWidget() {
	
}

void VisualizationWidget::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	
	painter.drawImage(QRect(0, 0, width(), height()), m_visualization->image());
}

void VisualizationWidget::resizeEvent(QResizeEvent *event) {
	m_visualization->resize(event->size());
	QWidget::resizeEvent(event);
	m_controller->fullVisualization();
	update();
}

void VisualizationWidget::mouseMoveEvent(QMouseEvent *event) {
	if(event->buttons().testFlag(Qt::LeftButton)) {
		m_controller->shift(event->globalPos() - m_lastMousePos);
		m_lastMousePos = event->globalPos();
	}
	QWidget::mouseMoveEvent(event);
}

void VisualizationWidget::mousePressEvent(QMouseEvent *event) {
	if(event->button() == Qt::LeftButton) {
		m_lastMousePos = event->globalPos();
	}
	QWidget::mousePressEvent(event);
}

void VisualizationWidget::mouseReleaseEvent(QMouseEvent *event) {
	QWidget::mouseReleaseEvent(event);
}
