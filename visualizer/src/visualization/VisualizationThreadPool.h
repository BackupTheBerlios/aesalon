#ifndef VisualizationThreadPool_H
#define VisualizationThreadPool_H

#include <QThreadPool>

class VisualizationThreadPool : public QThreadPool {
public:
	VisualizationThreadPool();
	virtual ~VisualizationThreadPool();
	
	static VisualizationThreadPool *singleton() { return &m_singleton; }
private:
	static VisualizationThreadPool m_singleton;
};

#endif
