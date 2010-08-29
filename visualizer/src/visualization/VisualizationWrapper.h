#ifndef VisualizationWrapper_H
#define VisualizationWrapper_H

#include "storage/DataCoord.h"
#include "storage/DataRange.h"

class VisualizationWrapper {
public:
	VisualizationWrapper();
	virtual ~VisualizationWrapper();
public:
	virtual const DataRange &range() const = 0;
	
	virtual void lock() = 0;
	virtual void unlock() = 0;
	virtual bool isLocked() const = 0;
	virtual void drawLine(DataCoord from, DataCoord to) = 0;
};

#endif
