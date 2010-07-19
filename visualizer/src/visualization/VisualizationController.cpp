#include <QThreadPool>

#include "VisualizationController.h"
#include "Visualization.h"

VisualizationController::VisualizationController(Module *module, Visualization *visualization)
	: m_module(module), m_visualization(visualization) {
	m_threadPool = VisualizationThreadPool::singleton();
	m_visualization->setController(this);
	qDebug("Max. threads: %i", m_threadPool->maxThreadCount());
}

VisualizationController::~VisualizationController() {

}

void VisualizationController::fullVisualization() {
	VisualizationRequest *request = new VisualizationRequest(m_module, m_visualization, m_visualization->range());
	/* Full visualization priority: 15. */
	m_threadPool->start(request, 15);
}

void VisualizationController::processRequest(VisualizationRequest *request) {
	/* Default priority: 10. */
	m_threadPool->start(request, 10);
}

void VisualizationController::renderRegion(const DataRange &range) {
	processRequest(new VisualizationRequest(m_module, m_visualization, range));
}

void VisualizationController::shift(QPoint pixels) {
	m_visualization->shift(pixels);
}

void VisualizationController::shift(DataRange range) {
	
}

void VisualizationController::scale(qreal zoom) {
	m_visualization->scale(zoom);
}
