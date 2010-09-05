#include "VisualizationRequest.h"
#include "module/Module.h"
#include "Visualization.h"
#include "VisualizationController.h"
#include "VisualizationWidget.h"

VisualizationRequest::VisualizationRequest(VisualizationController *controller, DataRange range, int category)
	: QRunnable(), m_controller(controller), m_range(range), m_abort(false), m_category(category) {
	
	setAutoDelete(true);
	m_controller->registerRequest(this);
}

VisualizationRequest::~VisualizationRequest() {
	m_controller->deregisterRequest(this);
}

void VisualizationRequest::run() {
	if(m_abort) return;
	
	Visualization *sv = m_controller->visualization()->subVisualization(m_range);
	/*qDebug("Asking renderer to render visualization . . .");*/
	m_controller->renderer()->renderRange(sv, m_range, &m_abort);
	if(!m_abort) {
		m_controller->visualization()->merge(sv);
		/*qDebug("VisualizationRequest executed, updating widget . . .");*/
		m_controller->widget()->update();
	}
	
	delete sv;
}
