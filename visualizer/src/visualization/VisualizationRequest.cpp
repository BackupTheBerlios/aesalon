#include "VisualizationRequest.h"
#include "module/Module.h"
#include "Visualization.h"

VisualizationRequest::VisualizationRequest(Module *module, Visualization *visualization, DataRange range)
	: QRunnable(), m_module(module), m_visualization(visualization), m_range(range) {
	
}

VisualizationRequest::~VisualizationRequest() {
	
}

void VisualizationRequest::run() {
	Visualization *sv = m_visualization->subVisualization(m_range);
	m_module->visualize(sv);
	m_visualization->merge(sv);
	delete sv;
}
