#include <QResizeEvent>
#include <QPaintEvent>
#include <QWheelEvent>

#include "VisualizationWidget.h"
#include "VisualizationWidget.moc"

VisualizationWidget::VisualizationWidget(Module *module, const QString &rendererName) : QWidget(NULL), m_module(module) {
	Renderer *renderer = module->renderer(rendererName.toStdString());
	m_visualization = new Visualization(size(), renderer->defaultRange());
	m_controller = new VisualizationController(module, m_visualization);
	m_controller->setWidget(this);
	m_controller->setRenderer(renderer);
	/* A resizeEvent will be generated once the widget has been added to a layout;
		when that happens, a full visualization will be produced. */
}

VisualizationWidget::~VisualizationWidget() {
	
}

void VisualizationWidget::paintEvent(QPaintEvent *event) {
	/* TODO: find a better method of doing this. Acquiring an exclusive lock
		on the mutex just to paint it is a bit overkill. */
	m_visualization->lock();

	QPainter painter(this);
	
	painter.drawImage(QRect(0, 0, width(), height()), m_visualization->image());
	m_visualization->unlock();
}

void VisualizationWidget::resizeEvent(QResizeEvent *event) {
	m_visualization->resize(event->size());
	QWidget::resizeEvent(event);
	m_controller->fullVisualization();
	update();
}

void VisualizationWidget::mouseMoveEvent(QMouseEvent *event) {
	if(event->buttons() & Qt::LeftButton) {
		m_controller->shift(event->globalPos() - m_lastMousePos);
		m_lastMousePos = event->globalPos();
		update();
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

void VisualizationWidget::wheelEvent(QWheelEvent *event) {
	qDebug("wheelEvent . . .");
	qreal scale = 1 - (event->delta() / 1000.0);
	qDebug("\tscale: %f", scale);
	
	if(event->modifiers() & Qt::ShiftModifier) {
		m_controller->scale(1.0, scale);
	}
	else if(event->modifiers() & Qt::ControlModifier) {
		m_controller->scale(scale, 1.0);
	}
	else {
		m_controller->scale(scale, scale);
	}
	
	update();
	
	QWidget::wheelEvent(event);
}

void VisualizationWidget::updateVisualization() {
	m_controller->updateVisualization();
}
