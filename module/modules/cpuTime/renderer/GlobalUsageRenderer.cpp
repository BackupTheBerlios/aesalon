#include <algorithm>
#include <iostream>

#include "GlobalUsageRenderer.h"

GlobalUsageRenderer::GlobalUsageRenderer(CpuTimeDataCache *dataCache) : AbstractRenderer<CpuTimeDataCache>(dataCache) {
	
}

GlobalUsageRenderer::~GlobalUsageRenderer() {
	
}

static bool compareCoords(const DataCoord &one, const DataCoord &two) {
	if(one.time() < two.time()) return true;
	return false;
}

void GlobalUsageRenderer::renderRange(VisualizationWrapper *visualization, const DataRange &range, bool *abort) {
	std::cout << "GlobalUsageRenderer::renderRange() . . ." << std::endl;
	const CpuTimeDataCache::DataVector &dataVector = dataCache()->dataVector();
	
	DataCoord c;
	
	c.setTime(range.beginTime());
	c.setData(0.0);
	
	CpuTimeDataCache::DataVector::const_iterator begin = std::lower_bound(dataVector.begin(), dataVector.end(), c, compareCoords);
	
	if(begin == dataVector.end()) {
		/* Then the visualization is out of range. */
		std::cout << "Out of range. " << std::endl;
		return;
	}
	else if(begin != dataVector.begin()) --begin;
	
	c.setTime(range.endTime());
	
	CpuTimeDataCache::DataVector::const_iterator end = std::upper_bound(dataVector.begin(), dataVector.end(), c, compareCoords);
	
	if(end == dataVector.begin()) {
		/* Then the visualization is out of range. */
		return;
	}
	else if(end != dataVector.end()) ++end;
	
	visualization->lock();
	
	for(; begin != (end-1); ++begin) {
		visualization->drawLine((*begin), *(begin + 1));
	}
	
	visualization->unlock();
	
	std::cout << end - begin << " elements in visualization." << std::endl;
}

DataRange GlobalUsageRenderer::defaultRange() const {
	const CpuTimeDataCache::DataVector &dataVector = dataCache()->dataVector();
	return DataRange(DataCoord(dataVector.front().time(), 0.0), DataCoord(dataVector.back().time(), 1.5));
}
