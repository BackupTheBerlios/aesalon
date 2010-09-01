#ifndef VisualizationWrapper_H
#define VisualizationWrapper_H

#include "renderer/DataCoord.h"
#include "renderer/DataRange.h"

class VisualizationWrapper {
public:
	virtual ~VisualizationWrapper() {}
public:
	virtual const DataRange &range() const = 0;
	
	virtual void clear() = 0;
	virtual void lock() = 0;
	virtual void unlock() = 0;
	virtual bool isLocked() const = 0;
	virtual void setPenColour(int r, int g, int b, int a) = 0;
	virtual void drawLine(DataCoord from, DataCoord to) = 0;
	virtual void drawBox(DataRange range) = 0;
};

#endif
