#include <QThreadPool>

#include "VisualizationController.h"
#include "Visualization.h"

VisualizationController::VisualizationController(Module *module, Visualization *visualization)
	: m_module(module), m_visualization(visualization) {
	m_threadPool = VisualizationThreadPool::singleton();
	m_visualization->setController(this);
}

VisualizationController::~VisualizationController() {

}

void VisualizationController::fullVisualization() {
	m_visualization->clear();
	VisualizationRequest *request = new VisualizationRequest(m_module, m_visualization, m_visualization->range());
	/* Full visualization priority: 15. */
	m_threadPool->start(request, 15);
}

void VisualizationController::processRequest(VisualizationRequest *request) {
	/* Default priority: 10. */
	m_threadPool->start(request, 10);
}

void VisualizationController::shift(QPoint pixels) {
	
}

void VisualizationController::shift(DataRange range) {
	
}
