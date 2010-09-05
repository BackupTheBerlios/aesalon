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

void VisualizationController::updateVisualization() {
	DataRange range = m_visualization->range();
	range.beginTime() = range.endTime();
	range.endTime() = m_module->latestHeartbeat();
	/*renderRegion(range, PartialRequest);*/
	qDebug("Update range: %lu %lu", range.beginTime(), range.endTime());
}

void VisualizationController::registerRequest(VisualizationRequest *request) {
	if(request->category() == FullRequest) {
		foreach(VisualizationRequest *request, m_requestSets[request->category()]) {
			request->abort();
		}
	}
	m_requestSets[request->category()].insert(request);
}

void VisualizationController::deregisterRequest(VisualizationRequest *request) {
	m_requestSets[request->category()].remove(request);
}

void VisualizationController::fullVisualization() {
	VisualizationRequest *request = new VisualizationRequest(this, m_visualization->range(), FullRequest);
	/* Full visualization priority: 15. */
	m_threadPool->start(request, 15);
}

void VisualizationController::renderRegion(const DataRange &range, RequestType type) {
	processRequest(new VisualizationRequest(this, range, type));
}

void VisualizationController::shift(QPoint pixels) {
	m_visualization->shift(pixels);
}

void VisualizationController::shift(DataRange range) {
	/* Handle the horizontal sides . . .*/
	m_visualization->lock();
	DataRange hRange = m_visualization->range();
}

void VisualizationController::scale(qreal zoom) {
	m_visualization->scale(zoom);
}

void VisualizationController::processRequest(VisualizationRequest *request) {
	/* Default priority: 10. */
	m_threadPool->start(request, 10);
}
