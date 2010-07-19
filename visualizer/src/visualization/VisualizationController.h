#ifndef VisualizationController_H
#define VisualizationController_H

#include <QThreadPool>
#include <QPoint>

#include "module/Module.h"
#include "VisualizationRequest.h"
#include "VisualizationThreadPool.h"

class VisualizationWidget;

class VisualizationController {
public:
	VisualizationController(Module *module, Visualization *visualization);
	virtual ~VisualizationController();
private:
	Module *m_module;
	Visualization *m_visualization;
	VisualizationThreadPool *m_threadPool;
	VisualizationWidget *m_widget;
public:
	Module *module() const { return m_module; }
	Visualization *visualization() const { return m_visualization; }
	VisualizationWidget *widget() const { return m_widget; }
	void setWidget(VisualizationWidget *widget) { m_widget = widget; }
	
	/** Requests a full visualization. This clears the visualization's pixmap
		and visualizes the entire data range. */
	void fullVisualization();
	/** Processes a VisualizationRequest.
		@param request The request to process. This memory should be
			dynamically-allocated, as it will be freed upon request
			completion.
	*/
	void processRequest(VisualizationRequest *request);
	
	void renderRegion(const DataRange &range);
	
	void shift(QPoint pixels);
	void shift(DataRange range);
};

#endif
