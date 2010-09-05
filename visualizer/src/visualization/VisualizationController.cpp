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
	Timestamp tdiff = m_module->latestHeartbeat() - range.endTime();
	
	shift(DataCoord(tdiff, 0.0));
	
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

void VisualizationController::shift(DataCoord by) {
	/* NOTE: locks removed due to possible deadlocks. */
	/* Handle the horizontal side . . .*/
	/*m_visualization->lock();*/
	DataRange hRange = m_visualization->range();
	if(by.time() < 0) {
		hRange.endTime() = hRange.beginTime();
		hRange.beginTime() += by.time();
	}
	else if(by.time() > 0) {
		hRange.beginTime() = hRange.endTime();
		hRange.endTime() += by.time();
	}
	/* Now for the vertical. */
	DataRange vRange = m_visualization->range();
	if(by.data() < 0) {
		vRange.endData() = vRange.beginData();
		vRange.beginData() += by.data();
	}
	else if(by.data() > 0) {
		vRange.beginData() = vRange.endData();
		vRange.endData() += by.data();
	}
	
	m_visualization->shift(by);
	
	renderRegion(hRange, PartialRequest);
	renderRegion(vRange, PartialRequest);
	/*m_visualization->unlock();*/
}

void VisualizationController::scale(qreal zoom) {
	m_visualization->scale(zoom);
}

void VisualizationController::processRequest(VisualizationRequest *request) {
	/* Default priority: 10. */
	m_threadPool->start(request, 10);
}
