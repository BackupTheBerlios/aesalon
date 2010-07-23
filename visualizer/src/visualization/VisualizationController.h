#ifndef VisualizationController_H
#define VisualizationController_H

#include <QThreadPool>
#include <QPoint>
#include <QSet>

#include "module/Module.h"
#include "VisualizationRequest.h"
#include "VisualizationThreadPool.h"

class VisualizationWidget;

class VisualizationController {
public:
	enum RequestType {
		FullRequest,
		PartialRequest,
		RequestTypes
	};

	VisualizationController(Module *module, Visualization *visualization);
	virtual ~VisualizationController();
private:
	Module *m_module;
	Visualization *m_visualization;
	VisualizationThreadPool *m_threadPool;
	VisualizationWidget *m_widget;
	
	typedef QSet<VisualizationRequest *> RequestSet;
	RequestSet m_requestSets[RequestTypes];
public:
	Module *module() const { return m_module; }
	Visualization *visualization() const { return m_visualization; }
	VisualizationWidget *widget() const { return m_widget; }
	void setWidget(VisualizationWidget *widget) { m_widget = widget; }
	
	void registerRequest(VisualizationRequest *request);
	void deregisterRequest(VisualizationRequest *request);
	
	/** Requests a full visualization. This visualizes the entire data range
		specified by the visualization. */
	void fullVisualization();
	
	void renderRegion(const DataRange &range, RequestType type = PartialRequest);
	
	void shift(QPoint pixels);
	void shift(DataRange range);
	
	void scale(qreal zoom);
private:
	/** Processes a VisualizationRequest.
		@param request The request to process. This memory should be
			dynamically-allocated, as it will be freed upon request
			completion.
	*/
	void processRequest(VisualizationRequest *request);
};

#endif
