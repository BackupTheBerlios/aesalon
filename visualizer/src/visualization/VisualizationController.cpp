#include <QThreadPool>

#include "VisualizationController.h"
#include "Visualization.h"

VisualizationController::VisualizationController(Module *module, Visualization *visualization)
	: m_module(module), m_visualization(visualization) {
	m_threadPool = VisualizationThreadPool::singleton();
	m_visualization->setController(this);
	qDebug("Max. threads: %i", m_threadPool->maxThreadCount());
	m_totalRange.beginData() = 1e50;
	m_totalRange.endData() = -1e50;
	m_totalRange.beginTime() = 0;
	m_totalRange.endTime() = 0;
}

VisualizationController::~VisualizationController() {

}

void VisualizationController::heartbeat(Timestamp timestamp) {
	if(timestamp > m_totalRange.endTime()) m_totalRange.endTime() = timestamp;
}

void VisualizationController::modifiedPoint(DataCoord point) {
	if(point.time() < m_totalRange.beginTime() || m_totalRange.beginTime() == 0) m_totalRange.beginTime() = point.time();
	if(point.time() > m_totalRange.endTime()) m_totalRange.endTime() = point.time();
	if(point.data() < m_totalRange.beginData()) m_totalRange.beginData() = point.data();
	if(point.data() > m_totalRange.endData()) m_totalRange.endData() = point.data();
}

void VisualizationController::updateVisualization() {
	DataRange range = m_visualization->range();
	Timestamp tdiff = m_module->latestHeartbeat() - range.endTime();
	
	heartbeat(m_module->latestHeartbeat());
	
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
	/*qDebug("Asked to shift visualization . . .");*/
	/* Invert the X value. The Y value is already inverted due to the visualization coordinate system. */
	pixels.setY(-pixels.y());
	
	pixels.setX(1);
	pixels.setY(0);
	
	shift(m_visualization->translateOffset(pixels));
}

void VisualizationController::shift(DataCoord by) {
	m_visualization->shift(by);

	/* Handle the horizontal side . . .*/
	DataRange hRange = m_visualization->range();
	if(by.time() < 0) {
		hRange.endTime() = hRange.beginTime();
		hRange.beginTime() += by.time();
		qDebug("** rendering horizontal region . . .");
		renderRegion(hRange, PartialRequest);
	}
	else if(by.time() > 0) {
		hRange.beginTime() = hRange.endTime();
		hRange.endTime() += by.time();
		qDebug("** rendering horizontal region . . .");
		renderRegion(hRange, PartialRequest);
	}
	/* Now for the vertical. */
	DataRange vRange = m_visualization->range();
	if(by.data() < 0) {
		vRange.endData() = vRange.beginData();
		vRange.beginData() += by.data();
		qDebug("** rendering vertical region . . .");
		renderRegion(vRange, PartialRequest);
	}
	else if(by.data() > 0) {
		vRange.beginData() = vRange.endData();
		vRange.endData() += by.data();
		qDebug("** rendering vertical region . . .");
		renderRegion(vRange, PartialRequest);
	}
	
	/*renderRegion(vRange, PartialRequest);*/
	/*renderRegion(m_visualization->range(), PartialRequest);*/
}

void VisualizationController::scale(qreal zoom) {
	m_visualization->scale(zoom);
}

void VisualizationController::processRequest(VisualizationRequest *request) {
	/* Default priority: 10. */
	m_threadPool->start(request, 10);
}
