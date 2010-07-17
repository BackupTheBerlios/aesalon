#ifndef RenderingThread_H
#define RenderingThread_H

#include <QThread>
#include <QSemaphore>
#include <QQueue>

class Module;
class Visualization;

class RenderingThreadRequest {
public:
	Module *module;
	Visualization *visualization;
	Visualization *parentVisualization;
};

class RenderingThread : public QThread {
public:
	RenderingThread();
	virtual ~RenderingThread();
private:
	QQueue<RenderingThreadRequest> m_requestQueue;
public:
	void addRequest(RenderingThreadRequest request);
	int requests() const { return m_requestQueue.size(); }
	
	virtual void run();
};

#endif
