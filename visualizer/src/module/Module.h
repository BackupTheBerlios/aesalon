#ifndef Module_H
#define Module_H

#include <QString>

#include "interface/Controller.h"
#include "storage/DataRange.h"

class Visualization;

class Module {
public:
	Module(const char *name);
	~Module();
private:
	QString m_name;
	void *m_moduleHandle;
	VisualizerModule::Controller *m_controller;
public:
	const QString &name() const { return m_name; }
	VisualizerModule::Controller *controller() const { return m_controller; }
	
	void processIncoming(DataPacket *packet);
	VisualizerModule::Renderer *createRenderer(std::string name);
	DataRange defaultDataRange();
private:
	QString modulePath(QString filename);
};

#endif
