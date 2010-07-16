#ifndef VisualizationWrapper_H
#define VisualizationWrapper_H

#include "storage/DataCoord.h"
#include "storage/DataRange.h"

class Visualization;

class VisualizationWrapper {
public:
	VisualizationWrapper(Visualization *visualization);
private:
	Visualization *m_visualization;
public:
	Visualization *visualization() const { return m_visualization; }
	const DataRange &dataRange() const;
	
	void lock() const;
	void unlock() const;
	bool isLocked() const;
	void drawLine(DataCoord from, DataCoord to) const;
};

#endif
