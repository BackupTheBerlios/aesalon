#ifndef VisualizationRequest_H
#define VisualizationRequest_H

#include <QRunnable>

#include "storage/DataRange.h"

class Module;
class Visualization;

class VisualizationRequest : public QRunnable {
public:
	VisualizationRequest(Module *module, Visualization *visualization, DataRange range);
	virtual ~VisualizationRequest();
private:
	Module *m_module;
	Visualization *m_visualization;
	DataRange m_range;
public:
	virtual void run();
};

#endif
