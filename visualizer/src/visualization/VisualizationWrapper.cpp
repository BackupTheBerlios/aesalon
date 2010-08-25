#include "VisualizationWrapper.h"
#include "Visualization.h"
#include "VisualizationController.h"

VisualizationWrapper::VisualizationWrapper(Visualization *visualization) : m_visualization(visualization) {

}

const DataRange &VisualizationWrapper::dataRange() const {
	return m_visualization->range();
}

void VisualizationWrapper::lock() const {
	m_visualization->lock();
}

void VisualizationWrapper::unlock() const {
	m_visualization->unlock();
}

bool VisualizationWrapper::isLocked() const {
	return m_visualization->isLocked();
}

void VisualizationWrapper::drawLine(DataCoord from, DataCoord to) const {
	m_visualization->drawLine(from, to);
}
