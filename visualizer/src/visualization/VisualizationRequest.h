#ifndef VisualizationRequest_H
#define VisualizationRequest_H

#include <QRunnable>

#include "storage/DataRange.h"

class VisualizationController;

class VisualizationRequest : public QRunnable {
public:
	VisualizationRequest(VisualizationController *controller, DataRange range, int category);
	virtual ~VisualizationRequest();
private:
	VisualizationController *m_controller;
	DataRange m_range;
	bool m_abort;
	int m_category;
public:
	void abort() { m_abort = true; }
	int category() const { return m_category; }
	virtual void run();
};

#endif
