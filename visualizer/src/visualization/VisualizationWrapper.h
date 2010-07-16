#ifndef VisualizationWrapper_H
#define VisualizationWrapper_H

#include "storage/DataCoord.h"

class Visualization;

class VisualizationWrapper {
public:
	VisualizationWrapper(Visualization *visualization);
private:
	Visualization *m_visualization;
public:
	Visualization *visualization() const { return m_visualization; }
	
	void lock();
	void unlock();
	bool isLocked();
	void drawLine(DataCoord from, DataCoord to);
};

#endif
