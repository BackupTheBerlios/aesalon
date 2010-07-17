#ifndef VisualizationController_H
#define VisualizationController_H

#include "module/Module.h"

#include <QThread>

class VisualizationController {
public:
	VisualizationController(Module *module, Visualization *visualization);
	virtual ~VisualizationController();
private:
	Module *m_module;
	Visualization *m_visualization;
public:
	Module *module() const { return m_module; }
	Visualization *visualization() const { return m_visualization; }
	
	
};

#endif
